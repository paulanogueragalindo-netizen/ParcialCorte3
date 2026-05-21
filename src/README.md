## Innovaciones implementadas

### 1. Gráficos geométricos CP437
Los tiles del mapa usan caracteres especiales del conjunto CP437 (bloques, medios bloques, símbolos) en lugar de letras simples, logrando una apariencia visual mejor en consola.  
> Referencia: [Code Page 437 — Wikipedia](https://en.wikipedia.org/wiki/Code_page_437) · [Roguelike rendering con CP437](http://roguebasin.com/index.php/Character_based_graphics)

### 2. Puertas visibles entre habitaciones
Las salidas se muestran como caracteres direccionales (`>` `<` `^` `v`) según su orientación en el mapa, indicando visualmente hacia dónde llevan.

### 3. Ocho habitaciones en cadena semi-lineal
El mapa tiene 8 habitaciones conectadas con una ruta principal y bifurcaciones opcionales. Cada habitación tiene nombre, tiles propios y salidas configuradas manualmente.

### 4. Colores en consola — multiplataforma
En Windows se usan las APIs de `SetConsoleTextAttribute` para colorear enemigos, objetos y la barra de vida. En Linux/Mac se usan códigos de escape ANSI (`\033[...m`).  
> Referencia: [ANSI escape codes — Wikipedia](https://en.wikipedia.org/wiki/ANSI_escape_code) · [Windows Console API — Microsoft Docs](https://learn.microsoft.com/en-us/windows/console/console-functions)

### 5. Audio por eventos
Se reproduce un tono distinto para cada acción (movimiento, ataque, daño, recoger objeto, victoria, derrota) usando `Beep()` en Windows y el carácter de alerta `\a` en Linux/Mac.  
> Referencia: [Beep() — Microsoft Docs](https://learn.microsoft.com/en-us/windows/win32/api/utilapiset/nf-utilapiset-beep)

### 6. Tercer enemigo: rata con movimiento aleatorio
La rata no persigue al jugador como el goblin y el esqueleto; se mueve dos pasos aleatorios por turno, haciendo su comportamiento impredecible.

### 7. Recogida automática de objetos
Al pisar una casilla con un objeto, el jugador lo recoge automáticamente sin necesidad de presionar ninguna tecla adicional. El látigo se equipa directo; la llave y la poción van al inventario.

### 8. Látigo con alcance de ataque a dos casillas
Si el jugador lleva el látigo equipado, puede atacar enemigos a distancia 2 en las cuatro direcciones cardinales, sin necesidad de estar adyacente.

### 9. Poción usable con tecla F
La poción se recoge automáticamente al pisarla pero no se consume sola. El jugador elige cuándo usarla presionando `F`, recuperando 30 puntos de vida hasta el máximo.

### 10. Refresco rápido de pantalla
En lugar de borrar la consola cada frame con `cls` / `clear`, se reposiciona el cursor en `(0,0)` con `SetConsoleCursorPosition` (Windows) o `\033[H` (Linux/Mac), eliminando el parpadeo.  
> Referencia: [SetConsoleCursorPosition — Microsoft Docs](https://learn.microsoft.com/en-us/windows/console/setconsolecursorposition)

### 11. Panel de estado y alerta de enemigo cercano
Debajo del mapa se muestra la vida actual, sala actual, inventario y si el jugador lleva el látigo. Si hay un enemigo a 2 casillas de distancia se activa una alerta en rojo.

### 12. Pantalla de título con lista de innovaciones
Al iniciar el juego se muestra una pantalla de bienvenida con el listado de características y una pantalla de controles antes de entrar al bucle principal.

### 13. Automatización de compilación — `build.c`
Script en C puro que detecta automáticamente el compilador disponible (CMake, MSVC o g++) y compila el proyecto sin que el usuario tenga que recordar los comandos.  
Compilar el script y ejecutarlo:
```bash
# Windows
cl build.c && build.exe

# Linux / macOS
gcc build.c -o build && ./build
```
Limpiar artefactos:
```bash
.\build.exe clean   # Windows
./build clean       # Linux/macOS
```

### 14. Puerta sellada con muro central (sala 4)
La sala 4 tiene un muro vertical interior con una puerta `+` que solo puede cruzarse si el jugador lleva la llave, obligando a explorar la rama de la sala 3 primero.

### 15. Altar visible en el santuario (sala 8)
La sala final tiene un altar formado por tiles `H` dispuestos en forma de cruz, que es el objetivo final del juego. Al pisarlo con la llave se activa la condición de victoria.