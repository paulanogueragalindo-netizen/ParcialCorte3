/* Windows: cl build.c && build.exe  |  Linux/macOS: gcc build.c -o build && ./build */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#define SEP "\\"
#define EXE "DungeonCrawler.exe"
#define NUL "nul"
#define DEVNULL "nul"
#else
#define SEP "/"
#define EXE "DungeonCrawler"
#define NUL "/dev/null"
#define DEVNULL "/dev/null"
#endif

static int ejecutar(const char* cmd) {
    int r;
    printf("+ %s\n", cmd);
    r = system(cmd);
    if (r != 0) {
        return 0;
    }
    return 1;
}

static int tiene_cmake(void) {
#ifdef _WIN32
    return ejecutar("cmake --version >" DEVNULL " 2>&1");
#else
    return ejecutar("cmake --version >" DEVNULL " 2>&1");
#endif
}

static int compilar_cmake(void) {
    if (ejecutar("cmake -S . -B build") == 0) {
        return 0;
    }
#ifdef _WIN32
    if (ejecutar("cmake --build build --config Release") == 0) {
        return 0;
    }
#else
    if (ejecutar("cmake --build build") == 0) {
        return 0;
    }
#endif
    return 1;
}

static int compilar_gcc(void) {
    const char* cmd =
        "g++ -std=c++11 -Wall -Iinclude -o " EXE " "
        "main.cpp "
        "src/Juego.cpp "
        "src/Mapa.cpp "
        "src/Jugador.cpp "
        "src/Enemigo.cpp "
        "src/Objeto.cpp "
        "src/Audio.cpp "
        "src/Consola.cpp "
        "src/Graficos.cpp";
    return ejecutar(cmd);
}

static int compilar_msvc_con_vcvars(const char* vcvars) {
    char cmd[2400];
    const char* fuentes =
        "main.cpp src\\Juego.cpp src\\Mapa.cpp src\\Jugador.cpp src\\Enemigo.cpp "
        "src\\Objeto.cpp src\\Audio.cpp src\\Consola.cpp src\\Graficos.cpp";

    sprintf(cmd,
        "cmd /c \"call \"%s\" >nul 2>&1 && "
        "cl /EHsc /nologo /Iinclude /Fe" EXE " %s\"",
        vcvars, fuentes);
    return ejecutar(cmd);
}

static int compilar_msvc(void) {
    const char* vcvars[] = {
        "C:\\Program Files\\Microsoft Visual Studio\\18\\Community\\VC\\Auxiliary\\Build\\vcvars64.bat",
        "C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\VC\\Auxiliary\\Build\\vcvars64.bat",
        "C:\\Program Files (x86)\\Microsoft Visual Studio\\2022\\BuildTools\\VC\\Auxiliary\\Build\\vcvars64.bat",
        "C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\BuildTools\\VC\\Auxiliary\\Build\\vcvars64.bat",
        0
    };
    int i;

    for (i = 0; vcvars[i] != 0; i++) {
        if (compilar_msvc_con_vcvars(vcvars[i]) == 1) {
            return 1;
        }
    }
    return 0;
}

static void limpiar(void) {
#ifdef _WIN32
    ejecutar("if exist build rmdir /s /q build");
    ejecutar("del /q *.obj 2>" DEVNULL);
    ejecutar("del /q " EXE " 2>" DEVNULL);
#else
    ejecutar("rm -rf build");
    ejecutar("rm -f *.o *.obj " EXE);
#endif
    printf("Limpieza lista.\n");
}

static void compilar(void) {
    printf("Dungeon Crawler - build.c\n\n");

    if (tiene_cmake() == 1) {
        printf("Usando CMake...\n");
        if (compilar_cmake() == 1) {
#ifdef _WIN32
            ejecutar("copy /Y build\\Release\\" EXE " . >" DEVNULL " 2>&1");
            ejecutar("copy /Y build\\Debug\\" EXE " . >" DEVNULL " 2>&1");
#endif
            printf("\nListo: " EXE "\n");
            return;
        }
    }

#ifdef _WIN32
    printf("CMake no disponible. Intentando MSVC (cl)...\n");
    if (compilar_msvc() == 1) {
        printf("\nListo: " EXE "\n");
        return;
    }
    printf("Intentando g++ (MinGW)...\n");
#else
    printf("CMake no disponible. Usando g++...\n");
#endif

    if (compilar_gcc() == 1) {
        printf("\nListo: " EXE "\n");
        return;
    }

    printf("\nERROR: no se pudo compilar.\n");
    printf("Instala CMake, g++, o Visual Studio Build Tools.\n");
    exit(1);
}

int main(int argc, char* argv[]) {
    if (argc > 1 && strcmp(argv[1], "clean") == 0) {
        limpiar();
        return 0;
    }
    compilar();
    return 0;
}
