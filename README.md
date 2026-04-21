# 🏙️ City Scenery - OpenGL Project

A beautiful animated city scenery built with C++ and OpenGL/GLUT.

![Project Screenshot](screenshot.png)

---

## 🚀 Algorithms Implemented

This project demonstrates the manual implementation of fundamental computer graphics algorithms:

1.  **DDA Line Drawing Algorithm**: Used for drawing dashed lane markings on the road.
2.  **Bresenham's Line Drawing Algorithm**: Used for solid road lines and car wheel spokes.
3.  **Midpoint Circle Drawing Algorithm**: Used for the Sun, Moon, Trees, Wheels, and Lamp glows.
4.  **2D Transformations**:
    - **Translation**: Car and cloud movement.
    - **Rotation**: Animated spinning wheels.
    - **Scaling**: Pulsing glow effect for the Sun and Moon.

---

## ✅ Requirements

- **Windows** (10 or 11)
- **MinGW (g++ compiler)**
- **GLUT / FreeGLUT** library

---

## 🛠️ Step 1: Install MinGW (g++ Compiler)

1. Download MinGW from: https://sourceforge.net/projects/mingw/
2. Install it (default path: `C:\MinGW`)
3. During setup, select:
   - `mingw32-gcc-g++`
   - `mingw32-base`
4. Click **Apply Changes**
5. Add MinGW to PATH:
   - Press `Win + S` → search **"Environment Variables"**
   - Click **"Edit the system environment variables"**
   - Click **"Environment Variables"**
   - Under **System Variables**, find `Path` → click **Edit**
   - Click **New** → type: `C:\MinGW\bin`
   - Click **OK** on all windows
6. Verify installation — open **Command Prompt** and type:
   ```
   g++ --version
   ```
   You should see the version number printed.

---

## 🛠️ Step 2: Install GLUT

MinGW usually comes with GLUT pre-installed. To verify:

- Check if this file exists: `C:\MinGW\include\GL\glut.h`
- Check if this file exists: `C:\MinGW\lib\glut32.lib`

If they are missing, download from:
https://www.transmissionzero.co.uk/software/freeglut-devel/

Then copy:
- `include\GL\*.h`  →  `C:\MinGW\include\GL\`
- `lib\*.lib`       →  `C:\MinGW\lib\`
- `bin\*.dll`       →  `C:\Windows\System32\`

---

## 🛠️ Step 3: Get the Project Files

Make sure you have these files in the same folder (e.g., `D:\City Scenary\`):

```
City Scenary\
├── main.cpp        ← main source code
├── compile.bat     ← build & run script
└── README.md       ← this file
```

---

## 🚀 Step 4: Compile and Run

### Option A — Double Click (Easiest)
1. Open the folder `D:\City Scenary\`
2. Double-click **`compile.bat`**
3. A window will open, compile, and launch the app automatically!

### Option B — Command Prompt (Manual)
1. Open **Command Prompt** (`Win + R` → type `cmd` → Enter)
2. Navigate to the project folder:
   ```
   cd "D:\City Scenary"
   ```
3. Compile:
   ```
   g++ main.cpp -o city_scenery.exe -lopengl32 -lglu32 -lglut32
   ```
4. Run:
   ```
   city_scenery.exe
   ```

---

## 🎮 Controls

| Key | Action |
|-----|--------|
| `N` | Toggle **Night / Day** mode |
| `ESC` | Quit the program |

---

## 🌆 Features

- 🌙 Animated night sky with 200 twinkling stars
- 🌛 Glowing moon with crescent and craters
- ☀️ Sun in day mode
- ☁️ Drifting clouds
- 🏢 26 layered buildings with lit windows
- 🛣️ Two-lane road with lane markings
- 🚗 4 animated cars (with headlights at night)
- 💡 Street lights with glow cones
- 🌳 Trees along the sidewalk
- 💧 Wet road reflections at night

---

## ❗ Troubleshooting

| Problem | Solution |
|---------|----------|
| `g++` not found | Add `C:\MinGW\bin` to system PATH |
| `cannot find -lglut32` | Install GLUT and copy files to MinGW |
| Black screen on launch | Update your graphics drivers |
| `.exe` not opening | Right-click → Run as administrator |

---

## 📁 Project Info

- **Language:** C++
- **Graphics API:** OpenGL + GLUT
- **Compiler:** MinGW g++
- **Platform:** Windows

---

*Made with ❤️ using OpenGL*
