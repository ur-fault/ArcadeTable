# ArcadeTable
Arcade table source code for school projet

## 3. výstup

V tomto výstupu jsem už dělal kód pro Arduino, zatím ne celou hru, nicměne nějakou abstrakci nad komponentami, jako obrazovka nebo ovládání.

Vytvořil jsem základní tři třídy na ovládání
- `Table` - celkový management komponent a jejich inicializace (`.setup()`)
- `Screen` - ovládaní obrazovky, věci jako
    - `.fill()` - vyplní obrazovku (všechny pixely) jednou barvou
    - `.setPixel()` - nastaví jeden pixel na barvu
    - `.getPixelIndex()` - převede z koordinátu `x` a `y` na index `0-99`
    - `.show()` - pošle screen buffer LEDkám
- `Controls` - ovládání hráčů, každý má svůj
    - `.isPressed()` - je switch hráče zmáčknut
    - `.getJoystick()` - vrací hodnoty `-1`, `0` a `1` pro obě osy, vrátí nenulovou hodnotu jen jednou za sebou, poté hráč musí zase nastavit neutrální pozici, než bude znovu registrována tato poloha. Okolo středu je tzv. dead zone 40%, která je registrovaná jako neutrální poloha


## časová náročnost

Různými částmi výstupu jsem se věnoval takto dlouho

| Aktivita                  | Strávený čas  |
| ------------------------- | ------------- |
| Příprava IDE (na Arduino) | 4.5h          |
| Arduino C++               | 3.5h          |
| - Seznámení               | 2h            |
| - Několik souborů         | 2h            |
| Abstrakce                 | 6.5h          |
| - Knihovny                | 2h            |
| - Screen                  | 3.5h          |
| - Controls                | 2h            |
| - Bootstrap               | 1h            |
| Pájení                    | 3h            |
| Celkem                    | 18h30min      |

---
Repository contents:
- [Base Snake for 2 players implemented in C++](https://github.com/ur-fault/ArcadeTable/tree/master/SnakeCpp). Just reference to look at when writing it for Arduino later
- [Snake for Arduino and framework for ArcadeTable](https://github.com/ur-fault/ArcadeTable/tree/vystup-3/Snake)

Not existing yet:
- Snake for Arcade Table
- "Dáma" for Arcade Table
