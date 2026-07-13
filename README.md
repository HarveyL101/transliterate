# Transliterate

A simple repo to maintain consistency in my dev environment across devices (Desktop <-> Laptop).

## Aims & Purpose

This software intends to provide a lightweight method of writing in-line cyrillic text, allowing the creation of personal notes or learning resources without having to copy + paste across multiple browser tabs, this would become exhausting in the case of alphabet tables for example.

This project's purpose originated out of a personal want, but could be useful to others hence it being made public.

> [!NOTE]
> This is a big WIP, and depending on the utilities available for use, this project may end up being better suited to windows or linux. If this preferentiality is avoidable it will be done so, but is still worth mentioning in the meantime.

## Installation & Usage

1. Clone the repo using `git clone https://Github.com/HarveyL101/transliterate <target_directory>`
    1. If already at the target directory, just replace `<target_directory` with `.`.
2. write markdown files into the `/src` directory, following the structure `<FILE_NAME>.src.md`.
3. Build the created files using the command `make transliterate`.
4. View the transliterated files within the `/build` directory. Then (optionally)...
    1. Drag-and-drop the files into a more convenient location using your file explorer.
    2. Move the root folder of your files from the terminal using `mv <current_directory> <target_directory>`
    3. Move single files using the terminal with `mv <current_directory>/<FILE_NAME>.md <target_directory>/<FILE_NAME>.md`

> [!IMPORTANT]
> Files that are intended to be translated but be within the `/src` directory AND end with `.src.md`, otherwise they will not be scanned by the program.
