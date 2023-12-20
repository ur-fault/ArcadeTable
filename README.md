# ArcadeTable
Arcade table source code for school projet

## 2. výstup
Během tohoto výstupu jsem dodělával Snaka v C++ běžícího na Linuxu v terminálu, zmeníl jsem vykreslování z jednoduchého `std:cout` na používání knihonvny curses.h, která se často používá na vytvýření TUI aplikací na Unixových platformách. Díky této knihovně mohou vývojáři upravovat plochu terminálu s mnohem větší kontrolou a efektivitou, rychlostí a jednoduchostí než manuálně. Dále se tato knihovna stará o zajištění vstupu od uživatele, takže Snake už jde ovládat a tím pádem i hrát. V poslední řadě jsem dodělal všechny herní mechaniky.

V tomto stavu je to plně hratelná hra Snake pro dva hráče, ale kvůli závislosti na knihovně curses.h podporuje jen Unixové systémy, tedy hlavně Linux a MacOS.

## obrázky
![arcade_table_snake_screenshot](https://github.com/ur-fault/ArcadeTable/assets/42314964/5c065aff-fd6c-4e49-98ea-c0fc5c73d3f9)
![arcade_table_snake_preview](https://github.com/ur-fault/ArcadeTable/assets/42314964/9ebaa3ce-b37b-4cdd-8a18-678333d44583)

## časová náročnost

Různými částmi výstupu jsem se věnoval takto dlouho

| Aktivita                 | Strávený čas  |
| ------------------------ | ------------- |
| ...                      | ...           |
|                          |               |
| Celkem                   | ...           |

---
Repository contents:
- [Base Snake for 2 players implemented in C++](https://github.com/ur-fault/ArcadeTable/tree/master/SnakeCpp). Just reference to look at when writing it for Arduino later

Not existing yet:
- Universal smoething of a framework for controlling LED RGB Matrix and handling user input on Arcade Table
- Snake for Arcade Table
- "Dáma" for Arcade Table
