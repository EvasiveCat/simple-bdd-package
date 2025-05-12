# simple-bdd-package

## Kompilieren

Das Packet wird mittels `make release` kompiliert und liegt dann als executable namens "main_release" vor.

## Überblick

Das Projekt ist so strukturiert:
 
```bash
├──benchmarks // Aiger Dateien
├──build // build fragmente
├──include // header files
├──log // Dotfiles die bei runtime entstehen (zB unterschiedliche outputs zweier circuits)
├──scripts // bash scripts zum wiederholen der Tests von Aufgabe 3
├──src // Der source code des Packets
├──main.cpp // Entry point des Programms und user interface
├──Makefile // Zum kompilieren
├──Programmieraufgabe.pdf // Die Aufgabenstellung
├──Protokoll.md // Ergebnisse von Aufgabe 3
└──README.md
```

### Scripts

Das Projekt kann in 3 verschiedenen Weise eingestellt werden. Zunächst wählt man DTL, dann Variablenordnung, dann was verglichen werden soll (Datei mit Datei, Datei mit Folder, Folder mit Folder).
Die scripts übernimmt dabei die Auswahl die man für Aufgabe 3 treffen muss.
Die Hauptskripte sind a.sh, b.sh, c1.sh, und c2.sh, die nach den jeweiligen Aufgaben benannt sind.

Die scripts nehmen an dass mit release kompiliert wurde und dass das executable "main_release" heißt.
Sie können aus dem root folder des Projekts einfach ausgeführt werden: zB: ./scripts/a.sh

Hilfsskripte:
- ./scripts/1-robdd_addierer.sh <executable_path> <folder1_path> <folder2_path>
        Führt das executable mit der Auswahl 1, 1, 1 aus. (BDD, Addierervariablenordnung, Datei mit Datei)
        Wird von a.sh ausgeführt
- ./scripts/2-robdd_zufall.sh <executable_path> <folder1_path> <folder2_path>
        Führt das executable mit der Auswahl 1, 2, 1 aus. (BDD, zufällige Variablenordnung, Datei mit Datei)
        Wird von b.sh ausgeführt
- ./scripts/3-kfdd_addierer.sh <executable_path> <folder1_path> <folder2_path>
        Führt das executable mit der Auswahl 4, 1, 1 aus. (KFDD, Addierervariablenordnung, Datei mit Datei)
        Wird von c1.sh ausgeführt
- ./scripts/3-kfdd_zufall.sh <executable_path> <folder1_path> <folder2_path>
        Führt das executable mit der Auswahl 4, 2, 1 aus. (KFDD, zufällige Variablenordnung, Datei mit Datei)
        Wird von c2.sh ausgeführt

### Src

```bash
src/
├── algorithms // Algorithmen aus der Vorlesung
│   ├── bddToKfdd.cpp // Konversion von BDD zu KFDD
│   ├── ite.cpp // ITE Algorithmus
│   └── kfddXOR.cpp // XOR Operator von zwei kfdds
├── bddNode.cpp // Funktionen auf bddNodes, vor allem operatoren *, +, ~, etc
├── dotPrint.cpp // Erstellen von dot Dateien für bddNodes
├── dtl.cpp // Funktionen über der Dekompositionstypliste
├── fileFinder.cpp // Utility zum Finden von Dateien und Pfaden
├── interface.cpp // Terminal interface output
├── parser.cpp // Parsing von Aiger Dateien
├── uniqueTable.cpp // Speichern von bddNodes, damit sie eindeutig bleiben
├── variableOrdering.cpp // Funktionen, die die Variablenordnung betreffen
└── verification.cpp // Verifikation von BDDs (Gleichheit/Simulation)
```