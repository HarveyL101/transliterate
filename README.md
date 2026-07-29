# RU-Transliterate

A lightweight transliteration program to embed cyrillic text within markdown files. Geared towards beginners and those not able to use a russian keyboard. Called using `\ru{lorem ipsum}`. 
## Aims & Purpose

This software intends to provide a lightweight method of writing in-line cyrillic text, allowing the creation of personal notes or learning resources without having to copy + paste across multiple browser tabs, this would become exhausting in the case of alphabet tables for example.

This project's purpose originated out of a personal want, but could be useful to others hence it being made public.

> [!NOTE]
> This is a big WIP, and depending on the utilities available for use, this project may end up being better suited to windows or linux. If this preferentiality is avoidable it will be done so, but is still worth mentioning in the meantime.

## Transliteration scheme

This program's `RULES[]` table is responsible for the mapping of Latin->Cyrillic.

| type | get | type | get | type | get |
|------|-----|------|-----|------|-----|
|   a  |  а  |   k  |  к  |  sh  |  ш  |
|   b  |  б  |   l  |  л  | shch |  щ  |
|   v  |  в  |   m  |  м  |   '  |  ь  |
|   g  |  г  |   n  |  н  |   "  |  ъ  |
|   d  |  д  |   o  |  о  |   y  |  ы  |
|   e  |  е  |   p  |  п  |   j  |  й  |
|  zh  |  ж  |   r  |  р  |  ya  |  я  |
|   z  |  з  |   s  |  с  |  yu  |  ю  |
|   i  |  и  |   t  |  т  |  yo  |  ё  |
|  ts  |  ц  |   u  |  у  |   c  |  к  |
|  ch  |  ч  |   f  |  ф  |   x  | кс  |
| kh/h |  х  |      |     |      |     |

> [!IMPORTANT]
> Known Ambiguities (Please fork the repo and edit `RULES[]` if these do not suit you)
>   - `y` → ы, `j` → й (avoids collision between the two)
>   - `c` → к; use `ts` for ц
>   - `e` → е; э is uncommon and unmapped by default, feel free to add your own rule if needed.

## Usage

### Build 

```shell
make 
```

### Use

### Automation


> [!IMPORTANT]
> Files that are intended to be translated but be within the `/src` directory AND end with `.src.md`, otherwise they will not be scanned by the program.
