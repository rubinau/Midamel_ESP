# Midamel_ESP
### Struktur File
```
root
|--lib
|  |
|  |--parsing
|  |  |- parsing.cpp
|  |  |- parsing.h
|  |
|  |- pin_controller.h
|  |- pin_unit.h
|  |
|  |--receive
|  |  |- receive.cpp
|  |  |- receive.h
|  |
|  |--send
|  |  |- send.cpp
|  |  |- send.h
|
|- platformio.ini
|--src
   |- main.cpp
```
## Setup
### 1. Library
```
pip install opencv-contrib-python
```
## Cara kontribusi
### 1. Clone Repo:
```
git clone https://github.com/rubinau/Lettux_Vision.git
```

### 2. Buat branch baru
```
git checkout -b <nama-branch>
# Misal <nama-branch> : bfs

git branch
# untuk check kita ada di branch apa
```
### 3. Add, commit, push
```
git add .
git commit -m "<msg>"
# add commit bisa dilakukan berkali2 secara lokal

git push origin <nama-branch>
# push ke remote repo
```
### 4. Pull Request
```
# Ini dilakukan 1 orang saja dengan pull request pada website github dan merge di VSCode jika ada conflict
```