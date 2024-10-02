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
  Hash:
- `f235145993e3873b040305f3944290fdd846260eaccad91f75b033efa95667b5`

- **Failo 2**:`bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb`  
  Hash:
- `1fbcb7987f604fe537e67fd4b29d27ca6590ac7fd054e505f606a5b3726707b4`

### 2. Testas
Du failai iš atsitiktinai sugeneruotų simbolių:

- **Pirmojo failo hash**:
- `feb7f36b9de99a9f8921a188481d6de5127c90595dfaed44835b03f652bf2ca7`
- **Antrojo failo hash**:
- `3125096c3a3e19ddf6c0c9fea922f4c7a4f5979a3e91592e7cae329c8d5f0738`

### 3. Testas
Du vienodi failai, viename skiriasi vienas ženklas:

- **Pirmojo failo hash**:
- `feb7f36b9de99a9f8921a188481d6de5127c90595dfaed44835b03f652bf2ca7`
- **Antrojo failo hash**:
- `304884b96baaa8a89765d0f8f7585826b010d6ce8bf9e0d0ab2cb87d67f85be5`

### 4. Testas
Tuščias failas:

- **Failo hash**: `85b1c46dfd68cb2c3ff989a39326692e8219241692e101d0c198ab148b3bc595`

### 5. Testas
**Konstitucija**

| Eiluciu sk. | Testas1 | Testas2 | Testas3 | Testas4 | Testas5 | Vidutinis |
|--------------|---------|---------|---------|---------|---------|-----------|
| 1            | 0       | 0       | 0       | 0       | 0       | 0         |
| 2            | 0       | 0       | 0       | 0       | 0       | 0         |
| 4            | 0       | 0       | 0       | 0       | 0       | 0         |
| 8            | 500100  | 0       | 0       | 0       | 500700  | 200160    |
| 16           | 501000  | 0       | 500200  | 500200  | 0       | 300280    |
| 32           | 500400  | 499600  | 500700  | 500300  | 500400  | 500280    |
| 64           | 1001100 | 1501300 | 1001100 | 1000900 | 1000300 | 1100940   |
| 128          | 2502200 | 2501700 | 2501900 | 3002400 | 2501800 | 2602000   |
| 256          | 6004700 | 6005200 | 6005100 | 6005000 | 6505600 | 6105120   |
| 512          | 13512100| 12010200| 13511300| 13511600| 13010900| 13111220  |
| 789          | 21017900| 21518200| 21518400| 21017800| 21518400| 21318140  |

![image](https://github.com/user-attachments/assets/267ab672-859c-43e0-8e96-931cccad4dab)


### 6. Testas
**Koliziju testas**

umber of collisions: 0
Total unique hashes: 100000
Rezultatas si karta nudziugino, koliziju neaptikta!! :)


