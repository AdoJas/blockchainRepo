# blockchainRepo

## Repozitorija, skirta blockchain modulio darbams

## Reikalavimai:

- Maišos funkcijos įėjimas (angl. input) gali būti bet kokio dydžio simbolių eilutė (angl. string).
- Maišos funkcijos išėjimas (angl. output) visuomet yra to paties, fiksuoto, dydžio rezultatas (pageidautina 256 bit'ų ilgio, t.y., 64 simbolių hex'as).
- Maišos funkcija yra **deterministinė**, t. y., tam pačiam įvedimui (input'ui) išvedimas (output'as) visuomet yra tas pats.
- Maišos funkcijos reikšmė/kodas (hash‘as) bet kokiai input'o reikšmei yra apskaičiuojamas greitai - efektyviai.
- Iš hash funkcijos rezultato (output'o) praktiškai neįmanoma atgaminti pradinio įvedimo (input'o).
- Maišos funkcija yra **atspari** ***"kolizijai"*** (angl. collision resistance), t.y., praktiškai neįmanoma surasti tokių dviejų skirtingų argumentų , kad jiems gautume tą patį hash'ą: H(x) != H(y), jei x != y.
- Bent minimaliai pakeitus įvedimą, pvz., vietoj "Lietuva" pateikus "lietuva", maišos funkcijos rezultatas-maišos kodas turi skirtis iš esmės, t.y., turi būti tenkinamas taip vadinamas lavinos efektas (angl. Avalanche effect).

## Pseudo-Kodas
```plaintext
//Pavyzdinis maišos funkcijos realizavimas naudojant rankinio ivedimo funkcija
FUNCTION manualHash()
    PRINT "Iveskite zodzius: "
    IGNORE previous input
    READ zodziai as string

    IF zodziai is empty THEN
        SET zodziai = "'m,ad/Kez**gqnI< sU 4esd;cx1GNwkF>}M,F_eJvTU)kw-yEt!:3}IC+e*J]YNC&L"
    END IF

    INITIALIZE hashArray to zeros
    SET previousY = 1

    FOR each character wc in zodziai DO
        SET decimalValue = ASCII value of wc
        CALL computeHashFunction(decimalValue, hashArray, previousY)
    END FOR

    SET finalHash = CALL toHexString(hashArray)
    PRINT "Hash: " + finalHash
END FUNCTION

FUNCTION computeHashFunction(x, hashArray, previousY)
    SET p1 = 2654435761
    SET p2 = 1597334677
    SET p3 = 2246822519
    SET p4 = 3266489917

    FOR i FROM 0 TO HASH_SIZE - 1 DO
        SET uniqueInput = (x XOR previousY) + (i * p1) //XOR pavyzdys - Jei bitai vienodi, grazina 0, jei skirtingi, grazina 1; 1011 XOR 1100 = 0111
        ROTATE uniqueInput LEFT by (i MOD 16)

        SET y = (13 * uniqueInput + 17 * uniqueInput^2 + 5 * previousY^2 + (previousY XOR uniqueInput) * p3 + (uniqueInput^3 MOD 31) * p4)
        SET y = (y XOR (y << 13)) XOR ((y >> 11) OR (previousY << (i MOD 8)))

        SET previousY = ((previousY * p1) XOR (y + i * p2) + (previousY << 5)) MOD 1048576

        UPDATE hashArray[i] with (y AND 0xFF)
        UPDATE hashArray[i] with ((y >> 8) AND 0xFF)
        UPDATE hashArray[i] with ((y >> 16) AND 0xFF)
        UPDATE hashArray[i] with ((y >> 24) AND 0xFF)
    END FOR
END FUNCTION

FUNCTION toHexString(hashArray)
    CREATE string stream oss
    FOR each byte in hashArray DO
        ADD byte in hex format to oss with padding
    END FOR
    RETURN oss as string
END FUNCTION
```

## Dabartinio kodo testavimas

### 1. Testas
Du failai sudaryti iš vieno, bet skirtingo simbolio:

- **Failo 1**:`aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa`  
  Hash: `f235145993e3873b040305f3944290fdd846260eaccad91f75b033efa95667b5`

- **Failo 2**:`bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb`  
  Hash: `1fbcb7987f604fe537e67fd4b29d27ca6590ac7fd054e505f606a5b3726707b4`

### 2. Testas
Du failai iš atsitiktinai sugeneruotų simbolių:

- **Pirmojo failo hash**: `feb7f36b9de99a9f8921a188481d6de5127c90595dfaed44835b03f652bf2ca7`
- **Antrojo failo hash**: `3125096c3a3e19ddf6c0c9fea922f4c7a4f5979a3e91592e7cae329c8d5f0738`

### 3. Testas
Du vienodi failai, viename skiriasi vienas ženklas:

- **Pirmojo failo hash**: `feb7f36b9de99a9f8921a188481d6de5127c90595dfaed44835b03f652bf2ca7`
- **Antrojo failo hash**: `304884b96baaa8a89765d0f8f7585826b010d6ce8bf9e0d0ab2cb87d67f85be5`

### 4. Testas
Tuščias failas:

- **Failo hash**: `85b1c46dfd68cb2c3ff989a39326692e8219241692e101d0c198ab148b3bc595`

### 5. Testas
**Konstitucija**

![image](https://github.com/user-attachments/assets/ca4f24a1-10d4-428f-88eb-5eae40a88842)

| Eiluciu sk | Laikas sec (vidutinis) |
|------------|------------|
| 1          | 0          |
| 2          | 0          |
| 4          | 0.000125025|
| 8          | 0.000375275|
| 16         | 0.000500575|
| 32         | 0.001125825|
| 64         | 0.002627475|
| 128        | 0.006130575|
| 256        | 0.0138871  |
| 512        | 0.032028325|
| 789        | 0.05133955 |

#### Seni rezultatai
| Eiluciu sk. | Vidutinis |
|--------------|-----------|
| 1            | 0         |
| 2            | 0         |
| 4            | 0         |
| 8            | 0.000200160    |
| 16           | 0.000300280    |
| 32           | 0.000500280    |
| 64           | 0.001100940   |
| 128          | 0.002602000   |
| 256          | 0.006105120   |
| 512          | 0.013111220  |
| 789          | 0.021318140  |

#### Matome, kad laikas suprastėjo, tačiau saugumas pagerėjo, o kolizijų nebėra. Manau, kad tai visai neblogas rezultatas.

### 6. Testas
**Determiniškumas**

- **Tas pats simbolių rinkinys turi generuoti tą patį hash'ą**
##### Reikšmės generuotos po 5 kartus
##### a reikšmė:
    1c1f23659273dcea20712ce8ab856f66a3ed39f690b12af904160e5168469f4b
#### lietuva reikšmė:
    aa8b872bcb70bc61eb3d62c05f6548033aea14b79dcee88bf4c1dc3bc4e27856
#### Lietuva reikšmė:
    c59d6f43d129b337f2764cdf1a24ea52809d8bd753466682c2621598ee15ec02
#### Lietuva! reikšmė:
    9f9e328c3e564d180783dea7e975e2b7069c8206923443e9af3e02efc86e1b51
#### lietuva! reikšmė:
    a1e1e069b9d131bfad5390d609d8a9ec8dd8b2f3fae4b1dc51f5994ec0054506

### 7. Testas
**Koliziju testas**

umber of collisions: 0
Total unique hashes: 100000
Rezultatas si karta nudziugino, koliziju neaptikta!! :)

### 7. Testas
**Sniego lavinos testas**
Testuota su 50 tūkstančių eilučių random string'u porų, kurios skiriasi vienu simboliu.

| Metric            | Average     | Minimum     | Maximum     |
|--------------------|-------------|-------------|-------------|
| Bit Differences     | 49.9812%   | 37.1094%    | 62.5%       |
| Hex Differences      | 93.7181%   | 78.125%     | 100%        |

Pagal šiuos rezultatus ir rastą informaciją internete, galima pasakyti, kad ši hašavimo funkcija atitinka sniego lavinos reikalavimus, vidutinis bitu skirtumas apie 50%, o hex skirtumas 94%.
Šie rezultatai nudžiugino, nes sniego lavinos testas yra vienas iš svarbiausių maišos funkcijų reikalavimų.

## Išvados

Atlikus tyrimą ir analizę, galima teigti, kad naujoji hash funkcija atitinka visus reikalavimus, kuriuos nurodėme pradžioje. Ji yra deterministinė, atspari kolizijai, efektyvi, atitinka sniego lavinos efektą ir turi fiksuoto dydžio rezultatą. Taip pat, ji yra atspari pakeitimams, nes net minimaliai pakeitus įvedimą, hash'as skiriasi iš esmės.
Nors hashinimo funkcija atitinka visus reikalavimus, tačiau ji nėra tokia saugi, kaip pvz. SHA-256, nes jos saugumas yra tiesiogiai proporcingas jos sudėtingumui. Tačiau, ši funkcija yra gera alternatyva, jei reikia greito hashinimo, o saugumas nėra pagrindinis reikalavimas.
Tai buvo puikus būdas prisiminti c++ kalbą ir atlikti įdomų tyrimą, kuris padėjo geriau suprasti hash funkcijas ir jų svarbą.

Rezultatai nuo pirminio bandymo skiriasi tuo, kad nesukuria kolizijų (bent jau su naudotais duomenimis), taip pat, sniego lavinos efektas yra geresnis nei buvo tikėtasi.


