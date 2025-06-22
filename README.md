![Screencastfrom2025-06-2208-50-43-ezgif com-video-to-gif-converter](https://github.com/user-attachments/assets/944abe0a-0ebd-48bd-902c-184df3c1c834)

# mycopy

A custom file copy utility for Linux that allows adjusting the buffer size for potentially faster file transfers.

## Why?

Larger buffer sizes during file copying can improve speed by reducing the number of read/write operations. This is especially useful when working with large files.

---

## Usage

### 1. Clone the repository

```bash
git clone https://github.com/Matin-Marzie/mycopy.git
cd mycopy
```

### 2. Create binary file

```bash
make
```

### 3. Run binary file

```bash
./mycopy -b65536 file1 file2
```
