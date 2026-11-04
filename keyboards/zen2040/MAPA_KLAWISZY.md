# Zen RP2040 – mapa klawiszy

Ściąga do szybkiego sprawdzenia, gdzie co jest. Szczegóły techniczne: [DOKUMENTACJA.md](DOKUMENTACJA.md).

## Jak włączać warstwy

| Co chcesz | Jak |
|---|---|
| Warstwa **NAV** (strzałki, F1–F12, RGB, media) | **przytrzymaj** `Caps*` (lewa, 3. rząd) **albo** `NAV` (lewy dolny rząd lub prawy dolny rząd) |
| Caps Lock | **stuknij** `Caps*` |
| Powrót do QWERTY | puść klawisz NAV – warstwa działa tylko podczas trzymania |

Na OLED-zie połówki z kablem USB widać nazwę aktywnej warstwy (`QWRTY` / `NAV`).

## Warstwa 0 – QWERTY (domyślna)

```
+------+------+------+------+------+------+                   +------+------+------+------+------+------+
|Esc ` |  1   |  2   |  3   |  4   |  5   |                   |  6   |  7   |  8   |  9   |  0   | Bksp |
+------+------+------+------+------+------+                   +------+------+------+------+------+------+
| Tab  |  Q   |  W   |  E   |  R   |  T   |                   |  Y   |  U   |  I   |  O   |  P   |  \   |
+------+------+------+------+------+------+                   +------+------+------+------+------+------+
|Caps* |  A   |  S   |  D   |  F   |  G   |                   |  H   |  J   |  K   |  L   |  ;   |  '   |
+------+------+------+------+------+------+------+     +------+------+------+------+------+------+------+
|Shift |  Z   |  X   |  C   |  V   |  B   | RGB< |     | RGB> |  N   |  M   |  ,   |  .   |  /   | Ent* |
+------+------+------+------+------+------+------+     +------+------+------+------+------+------+------+
| Ctrl | Alt  | Cmd  |RGBon | NAV  |                                 | NAV  |  -   |  =   | Cmd  | Alt  |
+------+------+------+------+------+------+------+     +------+------+------+------+------+------+------+
                                   | Bksp | Del  |     |Enter |Space |
                                   +------+------+     +------+------+
                                   | Bksp | Del  |     |Enter |Space |
                                   +------+------+     +------+------+
```

| Oznaczenie | Znaczenie |
|---|---|
| `Esc` (lewy górny) | Esc; z **Shift** daje `~`, z **Cmd** daje `` ` `` |
| `Caps*` | stuknięcie = Caps Lock, przytrzymanie = warstwa NAV |
| `Ent*` | stuknięcie = Enter, przytrzymanie = prawy Shift |
| `NAV` | warstwa NAV, dopóki trzymasz |
| `RGBon` | podświetlenie włącz / wyłącz |
| `RGB<` / `RGB>` | poprzedni / następny efekt podświetlenia |
| `Cmd` | ⌘ na Macu (klawisz Windows na PC) |
| `Alt` | ⌥ Option na Macu |
| Kciuki | górny i dolny rząd klawiszy kciuka mają te same funkcje |

## Warstwa 1 – NAV (trzymaj `Caps*` lub `NAV`)

```
+------+------+------+------+------+------+                   +------+------+------+------+------+------+
|  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |                   |  F7  |  F8  |  F9  | F10  | F11  | F12  |
+------+------+------+------+------+------+                   +------+------+------+------+------+------+
| RGB> | Sat+ | Jas+ | Sat- | BOOT |  [   |                   |  ]   | PgUp |  Up  | PgDn | Ins  | Home |
+------+------+------+------+------+------+                   +------+------+------+------+------+------+
|(trz) | Hue- | Jas- | Hue+ |      |      |                   |      | Left | Down |Right | Del  |S+Ent |
+------+------+------+------+------+------+------+     +------+------+------+------+------+------+------+
| Jas+ |      |      |      |      |      |      |     |      |      |      |      | Play | Prev | Next |
+------+------+------+------+------+------+------+     +------+------+------+------+------+------+------+
| Jas- | Cmd  | Alt  | RGB> |(trz) |                                 |(trz) | Left | Down |  Up  |Right |
+------+------+------+------+------+------+------+     +------+------+------+------+------+------+------+
                                   |      |      |     |      |      |
                                   +------+------+     +------+------+
                                   |      |      |     |      |      |
                                   +------+------+     +------+------+
```

| Oznaczenie | Znaczenie |
|---|---|
| `(trz)` | klawisz, który właśnie trzymasz, żeby być w NAV |
| puste pole | działa tak samo jak w QWERTY |
| `BOOT` | wejście w bootloader (wgrywanie firmware) |
| `S+Ent` | Shift+Enter (nowa linia bez wysyłania w komunikatorach) |
| `Play` / `Prev` / `Next` | multimedia: play/pauza, poprzedni, następny utwór |
| `Ins`, `Home` | Insert, Home |
| `Cmd`, `Alt` (dolny rząd lewej) | uwaga: w NAV są **zamienione miejscami** względem QWERTY |

### Podświetlenie RGB (wszystko w NAV)

| Klawisz w NAV | Pozycja w QWERTY | Działanie |
|---|---|---|
| `RGB>` | **Tab** oraz `RGBon` | następny efekt |
| `Jas+` / `Jas-` | **W** / **S**, a także Shift / Ctrl | jasność + / − |
| `Sat+` / `Sat-` | **Q** / **E** | nasycenie + / − |
| `Hue-` / `Hue+` | **A** / **D** | odcień (kolor) − / + |

Poza NAV: `RGBon` włącza/wyłącza diody, `RGB<` / `RGB>` (klawisze przy B i N) przełączają efekty.

Efekty (RGB Matrix): start na **Aurora**. Własne efekty są **na końcu listy** – najszybciej `RGB<` od stałego koloru:

| Efekt | Co robi | Prędkość / Odcień |
|---|---|---|
| **Aurora** | płynąca zorza na obu połówkach, jaśnieje przy szybkim pisaniu | dryf / kolor bazowy |
| **Embers** | klawisze rozgrzewają się przy pisaniu i stygną jak żar | stygnięcie / – |
| **Comet** | z każdego wciśnięcia lecą komety w lewo i prawo | prędkość / kolor tła |

Wbudowane: stały kolor, oddychanie, cykle tęczy (lewo-prawo, chevron, spirala, beacon), flower blooming, pixel rain/fractal, **typing heatmap**, **digital rain**, reaktywne (simple, multiwide, multinexus), splash, starlight, riverflow.

Wskaźniki: **Caps Lock** – klawisz `Caps*` świeci na biało. **NAV** – strzałki na biało, Play/Prev/Next na turkusowo.
Prędkość efektu w QMK nie ma osobnego klawisza w tej keymapie (do dodania przy pracy nad mapą klawiszy).
Ustawienia RGB zapamiętują się po odłączeniu. Gdy komputer śpi, diody gasną.

## Pokrętła (enkodery)

| Warstwa | Lewe pokrętło | Prawe pokrętło |
|---|---|---|
| QWERTY | przewijanie (scroll) góra / dół | głośność − / + |
| NAV | odcień RGB − / + | nasycenie RGB − / + |

## Wyświetlacze OLED

| Połówka | Co pokazuje |
|---|---|
| z kablem USB | ikona systemu (Apple / Windows / Linux), warstwa, NUMLK / CAPLK / SCRLK |
| druga | chomik w kółku – biegnie tym szybciej, im szybciej piszesz; licznik WPM i wykres z ostatniej minuty; śpi, gdy nie piszesz |

Ekrany gasną po 60 s bez pisania.

## Wgrywanie firmware – skrót

| Sposób | Jak |
|---|---|
| Z klawiatury | trzymaj `Caps*` + wciśnij **R** (`BOOT`) – tylko na lewej połówce |
| Przy podłączaniu | trzymaj **Esc** (lewa) albo **Bksp** (prawa) i podłącz USB |
| Sprzętowo | dwa razy szybko zewrzyj **RST** z **GND** na spodzie płytki |

Pojawia się dysk **RPI-RP2** → przeciągnij `zen2040_left.uf2` na lewą, `zen2040_right.uf2` na prawą. Zawsze **jedna połówka naraz, bez kabla TRRS**.
