# Dungeon Crawler — Parcial Corte 3

**Integrantes:** Paul Noguera · Julian Rojas

---

## ¿De qué trata el juego?

Juego de exploración por consola en C++. El jugador recorre ocho habitaciones de un calabozo, combate enemigos, recoge objetos y debe encontrar la llave para abrir el altar sagrado en la sala final y ganar.

---

## Innovaciones

1. Gráficos geométricos CP437 en consola
2. Puertas visibles entre habitaciones (`>` `<` `^` `v`)
3. Ocho habitaciones en cadena semi-lineal
4. Colores en consola (Windows) / símbolos legibles (Linux/Mac)
5. Audio por eventos (movimiento, ataque, daño, victoria…)
6. Tercer enemigo: rata con movimiento aleatorio
7. Recogida automática de objetos al pisarlos
8. Látigo con alcance de ataque a dos casillas
9. Poción usable con tecla `F`
10. Refresco rápido de pantalla sin parpadeo
11. Panel de estado y alerta de enemigo cercano
12. Pantalla de título con lista de innovaciones
13. Script `build.c` para compilación automática multiplataforma
14. Puerta sellada con muro central en sala 4
15. Altar visible como objetivo final en sala 8

---

## Compilación

**Con g++:**
```bash
g++ -std=c++11 -Iinclude -o DungeonCrawler main.cpp src/*.cpp
```

**Con el script automático** (detecta CMake, MSVC o g++ solo):
```bash
# Windows
cl build.c && build.exe

# Linux / macOS
gcc build.c -o build && ./build
```

---

## Ejecución

```bash
# Windows
.\DungeonCrawler.exe

# Linux / macOS
./DungeonCrawler
```

---

## Controles

| Tecla | Acción |
|---|---|
| `W A S D` | Mover |
| `J` / `Espacio` | Atacar |
| `F` | Usar poción |
| `T` | Soltar objeto |
| `Q` | Salir |
