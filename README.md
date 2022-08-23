# FF14 Tataru-helper-cli for LINUX

This is analog of [NightlyRevenger/TataruHelper](https://github.com/NightlyRevenger/TataruHelper) for linux for Final Fantasy 14.

## Requirements

Process: Xorg screenshot -> openCV -> ggl translate.

Install:

- for screenshot: X11, Xext

- for preprocessing img: opencv4

- for OCR: tesseract with installed lang libraries (from and to)

- for get-request to translate: cpr (curl wrapper)

- for loading yaml-config: yaml-cpp
  
  for compile: clang

## Usage

Run ./bin/tataru with path to your .yaml config:
`/home/user/tataru-ocr/bin/tataru "/home/user/tataru-ocr/CONFIG.yaml"`
