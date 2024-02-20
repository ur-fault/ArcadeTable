# ArcadeTable
Arcade table source code for school projet

## 4. výstup

Během tohoto výstupu jsem dodělával Snake hru pro Arduino a Dámu pro počítač.

Snake pro Arduino
- Vzal jsem abstrakci nad naším stolem a spojil ji dohromady s verzí pro počítač
- Přepis mnoho částí kódu, aby byl kompatibilní se standardní knihovnou Arduina, která je velice omezená
  - Minimalizovat heap alokace, které jsou na mikrokontrolerech drahé
  - Struktury místo tuplů
- Řešení mnoho chyb
  - Renderování
  - Input
  - Typos a prohozené piny

## časová náročnost

Různými částmi výstupu jsem se věnoval takto dlouho

| Aktivita                 | Strávený čas  |
| ------------------------ | ------------- |
| Příprava IDE             | 1h            |
| C++                      | 17h30min      |
| - Zprovoznění na Windows | 2h            |
| - Zprovoznění na Linux   | 30min         |
| - Seznámení              | 15h           |
| Začátek Snaka            | 5h            |
|                          |               |
| Celkem                   | 23h30min      |

---
Repository contents:
- [Base Snake for 2 players implemented in C++](https://github.com/ur-fault/ArcadeTable/tree/master/SnakeCpp). Just reference to look at when writing it for Arduino later

Not existing yet:
- Universal smoething of a framework for controlling LED RGB Matrix and handling user input on Arcade Table
- Snake for Arcade Table
- "Dáma" for Arcade Table
