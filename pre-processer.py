
import os
import sys
import shutil
import struct
import subprocess
import tempfile
import re


# config

UTMT = "UndertaleModCli.exe"
MAGIC = b"DSBS"
VERSION = 1


def clean_name(name):
    name = str(name)

    # windows doesnt like these
    name = re.sub(r'[<>:"/\\|?*]', "_", name)

    name = name.strip(" .")

    if not name:
        name = "sprite"

    return name


def get_data_win():
    if len(sys.argv) > 1:
        path = sys.argv[1].strip('"')

        if os.path.isfile(path):
            return os.path.abspath(path)

    print("put your data.win here pls")
    print()

    path = input("> ").strip().strip('"')

    if os.path.isfile(path):
        return os.path.abspath(path)

    return None


def make_utmt_script(path):
    script = r'''
using System;
using System.IO;
using UndertaleModLib.Util;

EnsureDataLoaded();

string root = Path.GetDirectoryName(ScriptPath);
string temp = Path.Combine(root, "exported");

if (Directory.Exists(temp))
    Directory.Delete(temp, true);

Directory.CreateDirectory(temp);

using (TextureWorker worker = new TextureWorker())
{
    for (int s = 0; s < Data.Sprites.Count; s++)
    {
        var sprite = Data.Sprites[s];

        if (sprite == null)
            continue;

        string name = sprite.Name?.Content;

        if (String.IsNullOrEmpty(name))
            name = "sprite_" + s;

        string dir = Path.Combine(
            temp,
            s.ToString("D6")
        );

        Directory.CreateDirectory(dir);

        File.WriteAllText(
            Path.Combine(dir, "name.txt"),
            name
        );

        for (int frame = 0;
             frame < sprite.Textures.Count;
             frame++)
        {
            var tex = sprite.Textures[frame];

            if (tex == null || tex.Texture == null)
                continue;

            string output = Path.Combine(
                dir,
                frame.ToString("D6") + ".png"
            );

            worker.ExportAsPNG(
                tex.Texture,
                output
            );
        }
    }
}
'''

    with open(path, "w", encoding="utf-8") as f:
        f.write(script)


def read_png(path):
    try:
        from PIL import Image
    except ImportError:
        print("error: pillow isnt installed")
        print("run: py -m pip install pillow")
        sys.exit(1)

    image = Image.open(path).convert("RGBA")

    return image.width, image.height, image.tobytes()


def make_sprite_bin(folder, output):
    name_file = os.path.join(folder, "name.txt")

    if os.path.isfile(name_file):
        with open(name_file, "r", encoding="utf-8") as f:
            name = f.read().strip()
    else:
        name = os.path.basename(folder)

    frames = []

    for filename in os.listdir(folder):
        if not filename.lower().endswith(".png"):
            continue

        path = os.path.join(folder, filename)

        try:
            width, height, pixels = read_png(path)

            frames.append({
                "file": filename,
                "width": width,
                "height": height,
                "pixels": pixels
            })

        except Exception as e:
            print("error: couldnt read", filename)
            print("       ", e)

    frames.sort(key=lambda x: x["file"])

    if not frames:
        return False

    # use the first frame size for the sprite header
    width = frames[0]["width"]
    height = frames[0]["height"]

    with open(output, "wb") as f:

        # dsbs header
        f.write(struct.pack(
            "<4sHHHH",
            MAGIC,
            VERSION,
            len(frames),
            width,
            height
        ))

        # sprite name
        name_bytes = name.encode("utf-8")

        f.write(struct.pack(
            "<H",
            len(name_bytes)
        ))

        f.write(name_bytes)

        # each frame
        for frame in frames:

            pixels = frame["pixels"]

            f.write(struct.pack(
                "<HHI",
                frame["width"],
                frame["height"],
                len(pixels)
            ))

            f.write(pixels)

    return True


def main():

    print("GAME MAKER TO BIN :3")

    data_win = get_data_win()

    if not data_win:
        print("error: couldnt find data.win")
        input()
        return

    script_dir = os.path.dirname(
        os.path.abspath(__file__)
    )

    utmt = os.path.join(
        script_dir,
        UTMT
    )

    if not os.path.isfile(utmt):
        print("error: UndertaleModCli.exe isnt here")
        print("       put it next to this script")
        input()
        return

    output_dir = os.path.join(
        os.path.dirname(data_win),
        "sprites"
    )

    if os.path.isdir(output_dir):
        shutil.rmtree(output_dir)

    os.makedirs(output_dir)

    temp_dir = tempfile.mkdtemp(
        prefix="gm_bin_"
    )

    try:

        script = os.path.join(
            temp_dir,
            "export.csx"
        )

        make_utmt_script(script)

        print("loading data.win...")

        result = subprocess.run(
            [
                utmt,
                "load",
                data_win,
                "-s",
                script
            ],
            cwd=temp_dir,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            text=True,
            encoding="utf-8",
            errors="replace"
        )

        if result.returncode != 0:

            print("error: undertalemodtool failed")
            print()

            # only show the useful part
            lines = result.stdout.splitlines()

            for line in lines:
                if "error" in line.lower():
                    print(line)

            input()
            return

        exported = os.path.join(
            temp_dir,
            "exported"
        )

        if not os.path.isdir(exported):
            print("error: no sprites were exported")
            input()
            return

        count = 0
        frames = 0

        for folder in os.listdir(exported):

            folder_path = os.path.join(
                exported,
                folder
            )

            if not os.path.isdir(folder_path):
                continue

            name_file = os.path.join(
                folder_path,
                "name.txt"
            )

            if os.path.isfile(name_file):

                with open(
                    name_file,
                    "r",
                    encoding="utf-8"
                ) as f:
                    name = clean_name(
                        f.read().strip()
                    )

            else:
                name = "sprite_" + folder

            output = os.path.join(
                output_dir,
                name + ".bin"
            )

            # dont overwrite another sprite with the same name
            if os.path.exists(output):

                n = 2

                while os.path.exists(
                    os.path.join(
                        output_dir,
                        name + "_" + str(n) + ".bin"
                    )
                ):
                    n += 1

                output = os.path.join(
                    output_dir,
                    name + "_" + str(n) + ".bin"
                )

            if make_sprite_bin(
                folder_path,
                output
            ):

                count += 1

                frames += len([
                    x for x in os.listdir(folder_path)
                    if x.lower().endswith(".png")
                ])

        print()
        print("done :3")
        print("sprites:", count)
        print("frames:", frames)
        print()
        print("saved to:")
        print(output_dir)

    finally:
        shutil.rmtree(
            temp_dir,
            ignore_errors=True
        )

    print()
    input("press enter to close...")


if __name__ == "__main__":
    main()

