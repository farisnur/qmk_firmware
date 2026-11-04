# Zen RP2040 – dokumentacja

Klawiatura split **RGBKB Zen rev2** przerobiona z ATmega32u4 (Pro Micro) na **RP2040 w formacie Pro Micro**.
Mapa klawiszy i skróty: [MAPA_KLAWISZY.md](MAPA_KLAWISZY.md).

## Spis treści
1. [Sprzęt](#1-sprzęt)
2. [Przypisanie pinów](#2-przypisanie-pinów)
3. [Funkcje firmware](#3-funkcje-firmware)
4. [Pliki i struktura](#4-pliki-i-struktura)
5. [Kompilacja](#5-kompilacja)
6. [Wgrywanie](#6-wgrywanie)
7. [Rozwiązywanie problemów](#7-rozwiązywanie-problemów)
8. [Historia i znane problemy](#8-historia-i-znane-problemy)
9. [Do zrobienia](#9-do-zrobienia)

---

## 1. Sprzęt

| Element | Opis |
|---|---|
| PCB | RGBKB Zen rev2, dwie połówki |
| Mikrokontroler | klon RP2040 „Pro Micro” (raster i rozkład pinów jak Pro Micro), osobny w każdej połówce |
| Połączenie połówek | kabel TRRS, **jeden przewód danych** (half-duplex serial) |
| Matryca | 5 rzędów × 7 kolumn na połówkę (34 pozycje), diody COL2ROW; obsadzone 32 klawisze (w tym podwójny klawisz kciuka) |
| Podświetlenie | 31 adresów WS2812 na połówkę: 29 pod klawiszami + 2 pary pod kciukiem (każda para świeci jak jedna dioda); pod klawiszem przy pokrętle brak diody |
| Wyświetlacze | OLED SSD1306 128×32 (I2C, adres `0x3C`), po jednym na połówkę, zamontowane pionowo |
| Enkodery | po jednym pokrętle na połówkę |

### Przeróbki względem oryginału
- **Diody:** w oryginalnym Zenie sygnał danych diod szedł przez TRRS (jeden łańcuch przez obie połówki).
  To połączenie zostało usunięte – **każda połówka steruje swoimi 31 diodami z własnego RP2040**.
  Efekty i kolory są synchronizowane programowo przez ten sam przewód, którym idą klawisze.
- **OLED:** linie SDA/SCL wyświetlacza trafiają na zamienione piny RP2040 (szczegóły w [sekcji 8](#8-historia-i-znane-problemy)).

> ⚠️ **Nie wpinaj ani nie wypinaj kabla TRRS przy podłączonym USB** – grozi uszkodzeniem mikrokontrolerów.

## 2. Przypisanie pinów

Mapowanie Pro Micro → RP2040 (identyczne na obu połówkach):

| Funkcja | Pin Pro Micro | Pin RP2040 |
|---|---|---|
| Kolumny 0–6 | F4, F5, F6, F7, B3, B1, B2 | GP29, GP28, GP27, GP26, GP20, GP22, GP23 |
| Rzędy 0–4 | C6, E6, B5, D7, B4 | GP5, GP7, GP9, GP6, GP8 |
| Enkoder A / B | D4 / D2 | GP4 / GP1 (prawa); lewa ma A/B zamienione – GP1 / GP4 – bo pokrętło jest tam wlutowane odwrotnie |
| Komunikacja TRRS | D3 | GP0 |
| Diody WS2812 | B6 | GP21 |
| OLED SDA | – | **GP3** (patrz sekcja 8) |
| OLED SCL | – | **GP2** (patrz sekcja 8) |

## 3. Funkcje firmware

### Połówki (master / slave)
- Połówka podłączona kablem USB jest „masterem” – ona wysyła klawisze do komputera. Do USB można podłączyć **dowolną** połówkę.
- Master wykrywany jest po tym, czy połówka nawiązała połączenie USB (klony RP2040 nie mają pinu do wykrywania zasilania USB). Czas oczekiwania: 2 s.
- **Strona (lewa/prawa) zapisana jest w pamięci EEPROM** każdej połówki (`EE_HANDS`). Zapisuje się przy pierwszym uruchomieniu po wgraniu pliku `_left` lub `_right`. Dzięki temu układ klawiszy jest poprawny niezależnie od tego, gdzie jest kabel.
- **Watchdog** (`split.transport.watchdog`): połówka, która uznała się za „drugą”, a przez 3 s nie dostaje danych, sama się restartuje (np. gdy po uśpieniu komputera obie połówki uznały się za drugie).
- Między połówkami synchronizowane są: aktywna warstwa, modyfikatory, stan Caps/Num Lock, włączenie OLED, WPM (tempo pisania) i stan RGB.

### Warstwy
- **QWERTY** (0) – podstawowa.
- **NAV** (1) – aktywna, dopóki trzymasz `Caps*` albo `NAV`. Zawiera F1–F12, strzałki (dwa zestawy: pod prawą ręką w stylu I/J/K/L oraz w prawym dolnym rogu), PgUp/PgDn/Home/Ins/Del, sterowanie RGB, multimedia i `BOOT`.
- Skróty z podwójną funkcją: `Caps*` (stuknięcie Caps Lock / trzymanie NAV), `Ent*` (stuknięcie Enter / trzymanie prawy Shift), `Esc` (Grave Escape: z Shift `~`, z Cmd `` ` ``).

### Podświetlenie RGB (RGB Matrix)
- **RGB Matrix**: każda dioda ma swoją pozycję na klawiaturze, więc efekty mogą reagować na wciśnięcia i rozchodzić się po obu połówkach.
- 2 × 31 diod. Mapa dioda ↔ klawisz (`g_led_config` w `zen2040.c`) została **zmierzona** keymapą `ledtest` (sekcja 7):
  na każdej połówce diody 0–28 idą rząd po rzędzie od zewnętrznej krawędzi, 29 = para pod lewą kolumną kciuka, 30 = para pod prawą kolumną kciuka.
  Oba klawisze z pary kciuka są przypisane do tej samej diody.
- Jasność maks. 120/255 (`rgb_matrix.max_brightness`). Powód: wszystkie diody zasilane są z jednego portu USB (0,5–0,9 A), a prąd drugiej połówki płynie przez styki TRRS. Przy 120 pełna biel to ok. 1 A w najgorszym razie; efekty kolorowe biorą dużo mniej.
- Efekt startowy: **Aurora**. Ustawienia (efekt, kolor, jasność, prędkość) zapisują się w EEPROM. Gdy komputer śpi, diody gasną.

**Efekty wbudowane QMK** (włączone): stały kolor, oddychanie, cycle left-right, cycle out-in dual, rainbow chevron, cycle spiral, rainbow beacon, flower blooming, pixel rain, pixel fractal, typing heatmap, digital rain, solid reactive (simple / multiwide / multinexus), multisplash, solid multisplash, starlight smooth, riverflow.

**Efekty własne** (`keymaps/default/rgb_matrix_user.inc`, na końcu listy – najszybciej przez `RGB<`):

| Efekt | Opis | Prędkość | Odcień |
|---|---|---|---|
| **Aurora** | dwie nakładające się fale światła płyną przez obie połówki; im szybciej piszesz (WPM), tym jaśniej i bardziej nasycone | tempo dryfu | kolor bazowy (±48) |
| **Embers** | każde wciśnięcie rozgrzewa klawisz i okolicę; żar stygnie przez czerwień → pomarańcz → żółć → biel; w spoczynku pojedyncze iskry | tempo stygnięcia | – (paleta ognia) |
| **Comet** | z wciśniętego klawisza wylatują dwie komety w lewo i prawo wzdłuż rzędu, przez obie połówki; każdy klawisz ma swój kolor | prędkość komet | kolor bazowy i tła |

**Wskaźniki** (rysowane na każdym efekcie, `keymap.c`): Caps Lock → klawisz `Caps*` na biało; warstwa NAV → strzałki na biało, multimedia (Play/Prev/Next) na turkusowo.

### Enkodery
Zdefiniowane jako *encoder map* (zależne od warstwy): QWERTY – lewy przewija, prawy zmienia głośność; NAV – lewy odcień, prawy nasycenie RGB.
Rozdzielczość domyślna (4 impulsy na ząbek) – zmierzona: jeden ząbek = jeden krok. Kierunek lewego poprawiony zamianą pinów A/B (patrz tabela pinów).

### OLED
- **Master** (połówka z USB): ikona systemu wykrytego automatycznie (Apple / Windows / Linux / inne) – bitmapa 32×32 na całą szerokość ekranu (`os_icons.h`), nazwa warstwy, wskaźniki NUMLK / CAPLK / SCRLK.
  Wykrywanie systemu (`OS_DETECTION`) działa przy podłączeniu i może potrzebować ok. 1 s.
- **Slave** (druga połówka): animacja chomika w kółku:
  - prędkość kółka i przebierania łapek rośnie z WPM (pełna prędkość ok. 135–150 WPM),
  - licznik WPM,
  - wykres tempa: 32 słupki co 2 s (ostatnie ~64 s), skala do 120 WPM,
  - przy WPM = 0 chomik śpi, nad nim unoszą się „z”.
- Ekrany gasną po 60 s bez aktywności (`OLED_TIMEOUT`), odświeżanie co 50 ms.

### Inne
- **Tap-hold** (`keymaps/default/config.h`): `Ent*` służy też jako Shift do wielkich liter, więc:
  `PERMISSIVE_HOLD` (klawisz wciśnięty i puszczony w trakcie trzymania `Ent*`/`Caps*` = Shift/NAV, nawet przed 200 ms),
  `CHORDAL_HOLD` (szybkie `Ent*` + klawisz prawej ręki = Enter, lewej = Shift; `Caps*` z tej reguły wyłączony),
  `QUICK_TAP_TERM 120` (stuknięcie i przytrzymanie w ciągu 120 ms powtarza Enter, później to Shift),
  `SPECULATIVE_HOLD` (Shift z `Ent*` działa od razu, np. Shift+klik). Celowo **bez** Flow Tap – zamieniałby „spacja, `Ent*`+litera” w Enter.
  Do przetestowania w praktyce.
- **Debounce `sym_defer_pk`, 5 ms**: osobno dla każdego klawisza; zmiana stanu jest wysyłana dopiero, gdy styk jest stabilny przez 5 ms. Próbowany `sym_eager_pk` (wysyłanie natychmiastowe) dawał podwójne wciśnięcia na A, 0 i P – przełączniki drgają przy puszczaniu.
- **NKRO** włączone domyślnie – dowolna liczba jednocześnie wciśniętych klawiszy. Ustawienie startowe zapisuje się w EEPROM; po wgraniu metodą „trzymaj górny klawisz przy podłączaniu” EEPROM jest czyszczony i NKRO włącza się samo.
- **Bootmagic**: trzymanie lewego górnego klawisza (lewa) lub prawego górnego (prawa) podczas podłączania USB → bootloader.
- **Podwójny reset**: dwa szybkie resety w ciągu 500 ms → bootloader.
- Mouse keys i klawisze multimedialne włączone.

## 4. Pliki i struktura

Repozytorium: fork `github.com/farisnur/qmk_firmware`, lokalnie `~/Projects/QMK firmware/qmk_farisnur`.
Zawiera tylko QMK i własne klawiatury (`keyboards/zen2040`, `keyboards/void30`). Aktualizacja QMK: `./util/sync_upstream.sh` w katalogu repo.
Stare repo tylko do wglądu: `~/Projects/QMK firmware/qmk_old`.

```
keyboards/zen2040/
├── keyboard.json          sprzęt: piny, matryca, RGB Matrix, split, układ klawiszy
├── config.h               EE_HANDS, wykrywanie USB, watchdog, piny OLED, limit jasności
├── rules.mk               własny transport OLED (programowe I2C)
├── zen2040.c              mapa diod (g_led_config) i ekran statusu mastera
├── oled_sw_i2c.c          programowe I2C dla OLED (obejście zamienionych pinów)
├── os_icons.h             ikony systemów 32×32 dla OLED
├── glcdfont.c             font OLED (z ikonami systemów, pochodzi z rgbkb/zen)
├── build.sh               buduje oba pliki .uf2
├── MAPA_KLAWISZY.md       ściąga z mapą klawiszy
├── DOKUMENTACJA.md        ten plik
└── keymaps/
    ├── default/
    │   ├── keymap.c       warstwy, enkodery, wskaźniki RGB, nazwy warstw na OLED
    │   ├── rgb_matrix_user.inc  własne efekty: Aurora, Embers, Comet
    │   ├── hamster.c/.h   animacja chomika
    │   ├── config.h       synchronizacja WPM, odświeżanie OLED, efekt startowy
    │   └── rules.mk       encoder map, WPM
    ├── diag/              diagnostyka: skan I2C w konsoli
    └── ledtest/           diagnostyka: kolejność diod i impulsy pokręteł
```

Gotowe pliki do wgrania: `~/Projects/QMK firmware/firmware_out/zen2040_left.uf2` i `zen2040_right.uf2`.

### Gdzie co zmieniać
| Chcę zmienić… | Plik |
|---|---|
| klawisz / warstwę | `keymaps/default/keymap.c` |
| funkcje pokręteł | `encoder_map` w `keymaps/default/keymap.c` |
| wygląd ekranu mastera | `zen2040.c` (`render_status`) |
| animację chomika, skalę wykresu | `keymaps/default/hamster.c` (stałe na górze pliku) |
| włączone efekty, jasność maks. | `keyboard.json` → `rgb_matrix` |
| własne efekty | `keymaps/default/rgb_matrix_user.inc` |
| mapa diod | `zen2040.c` → `g_led_config` |
| wskaźniki (Caps, NAV) | `keymaps/default/keymap.c` → `rgb_matrix_indicators_advanced_user` |
| maks. jasność, czas gaszenia OLED | `config.h` |

## 5. Kompilacja

Wymagania: `qmk` CLI (Homebrew) i `arm-none-eabi-gcc`.

```bash
cd ~/Projects/QMK\ firmware/qmk_farisnur
./keyboards/zen2040/build.sh
cp zen2040_left.uf2 zen2040_right.uf2 ../firmware_out/
```

`build.sh` buduje firmware dwa razy, z flagą `INIT_EE_HANDS_LEFT` i `INIT_EE_HANDS_RIGHT`. Te pliki różnią się tylko tym, którą stronę zapisują w EEPROM.

Sprawdzenie poprawności definicji: `qmk lint -kb zen2040 -km default`.

> Gdyby `qmk` zgłaszał `Could not find module appdirs`:
> `/opt/homebrew/Cellar/qmk/<wersja>/libexec/bin/python -m pip install appdirs`

## 6. Wgrywanie

1. Odłącz kabel TRRS. Wgrywaj **jedną połówkę naraz**.
2. Wprowadź połówkę w bootloader (dowolny sposób):
   - `Caps*` + **R** (tylko lewa, gdy działa firmware),
   - trzymaj lewy górny (lewa) / prawy górny (prawa) klawisz podczas podłączania USB,
   - zewrzyj dwa razy szybko **RST** z **GND** (spód płytki, rząd RAW · GND · RST · VCC).
3. Pojawi się dysk **RPI-RP2** – skopiuj na niego plik:
   - lewa → `zen2040_left.uf2`,
   - prawa → `zen2040_right.uf2`.
4. Dysk sam zniknie, połówka uruchomi się z nowym firmware.
5. Po obu połówkach: odłącz USB, podłącz TRRS, podłącz USB.

> ❗ Wgranie pliku `_right` na lewą połówkę (lub odwrotnie) zapisze złą stronę – klawisze z drugiej połówki będą „lustrzane”. Naprawa: wgraj właściwy plik ponownie.

Po zmianie samej keymapy można wgrywać te same pliki `_left`/`_right` – nic się nie psuje.

## 7. Rozwiązywanie problemów

| Objaw | Przyczyna / co zrobić |
|---|---|
| Druga połówka nie działa | Sprawdź TRRS (odłącz USB!). Obie połówki muszą mieć aktualny firmware. |
| Klawisze drugiej połówki dają złe znaki (np. U daje r) | Zła strona w EEPROM – wgraj właściwy plik `_left`/`_right` na tę połówkę. |
| OLED czarny | Po 60 s bez pisania gaśnie – naciśnij klawisz. Jeśli dalej czarny: keymapa `diag` (sekcja niżej). |
| Ikona złego systemu | Odłącz i podłącz USB – wykrywanie działa tylko przy podłączeniu. |
| Diody migają / pierwsza dioda dziwnie | RP2040 daje sygnał 3,3 V, a diody działają na 5 V – na granicy tolerancji. Ewentualnie konwerter poziomów lub dioda „poświęcona” jako bufor. |
| Część diod nie świeci | Sprawdź lutowanie. Dioda z uszkodzonym świeceniem zwykle dalej przekazuje sygnał, więc kolejne świecą. Potwierdź keymapą `ledtest`. |
| Jasność „skacze” schodkami przy powolnych falach | Diody mają tylko 256 poziomów; w ciemnym zakresie kroki są widoczne. Własne efekty trzymają jasność w górnej połowie zakresu; wbudowane efekty schodzące do ciemności (np. oddychanie) mogą to pokazywać. |
| Podwójne litery | Drgający przełącznik. Debounce to `sym_defer_pk` 5 ms; można podnieść `DEBOUNCE` lub wymienić przełącznik. |
| Pokrętło kręci odwrotnie | Zamień `pin_a`/`pin_b` tej połówki w `keyboard.json` (`encoder` = lewa, `split.encoder.right` = prawa). |
| Firmware nie startuje po wgraniu | Wejdź w bootloader i wgraj ponownie. |

### Diagnostyka (keymapa `diag`)
Keymapa wypisuje w konsoli QMK wynik skanowania magistrali I2C co 3 s (programowo, oba warianty SDA/SCL).

```bash
qmk compile -kb zen2040 -km diag -e TARGET=zen_diag_left -e EXTRAFLAGS=-DINIT_EE_HANDS_LEFT
# wgraj zen_diag_left.uf2, potem:
qmk console
```
Poprawny wynik: `0x3C` w jednym z wariantów. Po diagnostyce wgraj z powrotem normalny firmware.

### Kolejność diod i pokrętła (keymapa `ledtest`)
Zapala po jednej diodzie (numer na OLED-zie); wciśnij klawisz, pod którym świeci. Pokrętło: jeden ząbek w prawo = pomiń diodę, w lewo = cofnij (na obu połówkach). Każdy impuls pokrętła jest logowany.

```bash
qmk compile -kb zen2040 -km ledtest -e TARGET=zen2040_ledtest_left -e EXTRAFLAGS=-DINIT_EE_HANDS_LEFT
qmk compile -kb zen2040 -km ledtest -e TARGET=zen2040_ledtest_right -e EXTRAFLAGS=-DINIT_EE_HANDS_RIGHT
# wgraj na jedną połówkę (bez TRRS), potem:
qmk console
```
Wynik: linie `[led] N -> row,col` i na koniec pełna lista. Wejście w bootloader z tego firmware: trzymaj skrajny górny klawisz przy podłączaniu USB.

## 8. Historia i znane problemy

### Co nie działało w starej wersji (stare repo, styczeń 2025)
1. **Enkodery i OLED wyłączone** – `rgbkb/zen/rules.mk` miał `ENCODER_ENABLE = no` i `OLED_ENABLE = no`, co nadpisywało ustawienia z `keyboard.json`.
2. **Brak konfiguracji I2C** dla RP2040.
3. **Sprzeczne ustawienia strony** – jednocześnie `EE_HANDS` i `MASTER_LEFT`, a obie połówki dostawały ten sam plik. W efekcie prawa myślała, że jest lewą.
4. **RGB bez podziału na połówki** – brak synchronizacji efektów; do tego założone 34 diody zamiast faktycznych 31.
5. **`RP2040_FLASH_W25X10CL`** – ustawienie dla innej kości pamięci flash; usunięte.
6. Przestarzałe kody `RGB_MOD/VAI/VAD` → zamienione na `UG_*`.

### OLED – zamienione SDA/SCL
Diagnostyka wykazała, że wyświetlacz odpowiada (`0x3C`) tylko przy **SDA = GP3, SCL = GP2**.
Sprzętowe I2C w RP2040 nie może mieć SDA na GP3, dlatego OLED obsługuje programowe I2C (`oled_sw_i2c.c`, `OLED_TRANSPORT = custom`).
Działa poprawnie; jedyna wada to minimalnie większe obciążenie procesora przy odświeżaniu ekranu.

## 9. Do zrobienia

- [ ] **Przelutować zworki na PCB**, żeby OLED miał SDA → GP2, SCL → GP3, i wrócić do sprzętowego I2C – zgodnie ze sztuką:
  1. potwierdzić keymapą `diag` (`0x3C` w wariancie `SDA=GP2 SCL=GP3`),
  2. usunąć `oled_sw_i2c.c`, `OLED_TRANSPORT = custom`, `SRC += oled_sw_i2c.c` i linię `CFLAGS` z `rules.mk`,
  3. w `config.h` zamienić `OLED_SW_*` na `I2C_DRIVER I2CD1`, `I2C1_SDA_PIN GP2`, `I2C1_SCL_PIN GP3`,
  4. dodać `halconf.h` (`#define HAL_USE_I2C TRUE` + `#include_next <halconf.h>`) i `mcuconf.h` (`#include_next <mcuconf.h>`, `#undef RP_I2C_USE_I2C1`, `#define RP_I2C_USE_I2C1 TRUE`),
  5. usunąć keymapę `diag`, zbudować i wgrać obie połówki.
- [ ] Rozważyć konwerter poziomów 3,3 V → 5 V dla linii danych diod, jeśli pojawią się problemy z RGB.
- [ ] Potwierdzić diody pod **9**, **O** i **'** na prawej połówce (w teście przeskoczone przez podwójne wciśnięcia; mapa przyjęta z symetrii z lewą).
- [ ] Dalsza praca nad efektami RGB.
