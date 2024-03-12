# ArcadeTable
Arcade table source code for school projet

## 4. výstup

Během tohoto výstupu jsem dodělával Snake hru pro Arduino a Dámu pro počítač. Také jsem udělal testovací kód, pomocí kterého můžeme testovat, že všechen hardware funguje správně.

Snake pro Arduino
- Vzal jsem abstrakci nad naším stolem a spojil ji dohromady s verzí pro počítač
- Přepis mnoho částí kódu, aby byl kompatibilní se standardní knihovnou Arduina, která je velice omezená
  - Minimalizovat heap alokace, které jsou na mikrokontrolerech drahé
  - Struktury místo tuplů
- Řešení mnoho chyb
  - Renderování
  - Input
  - Typos a prohozené piny
  - Debug session přes chat

Testovací program

Využívá stejný framework kód jako Snake a v budoucnu dáma. Testuje jak input (switche a joysticky), tak i output (obrazovku).
- Pixely najednou a jejich barvy
  - Bílou
  - Červenou
  - Zelenou
  - Modrou
- Indexování pixelů
  - Index
  - Souřadnice
- Switche - oba hráči
- Joystick - oba hráčí


## časová náročnost

Různými částmi výstupu jsem se věnoval takto dlouho

| Aktivita                 | Strávený čas  |
| ------------------------ | ------------- |
| **Portovat PC verzi**    | **19h 30m**   |
| - Kompatibilita          | 4h 30m        |
| - Optimalizace           | 3h 30m        |
| - STD přepis             | 3h            |
| - debugging              | 8h 30m        |
| **Testovací program**    | **4h 30m**    |
| - Základ                 | 1h            |
| - Display                | 1h 30m        |
| - Input                  | 2h            |
|                          |               |
| Celkem                   | 24h           |

---
Repository contents:
- [Snake pro Arcade stul](https://github.com/ur-fault/ArcadeTable/tree/vystup-4/Snake). Implementovaný Snake pro Arcade table
