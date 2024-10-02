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

## Testavimas

### 1. Testas
Du failai sudaryti iš vieno, bet skirtingo simbolio:

- **Failo 1**:`aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa`  
  Hash:
- `2b2f9929dfbbf9590b075979a76bf959dbff89794f8b2969db67a949079b4949`

- **Failo 2**:`bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb`  
  Hash:
- `41b11f8f31e17beb5191c7a77131cbdb81116f3f11a10b1b51b187a7f1111beb`

### 2. Testas
Du failai iš atsitiktinai sugeneruotų simbolių:

- **Pirmojo failo hash**:
- `4e5b9136cc395fa4ba57cdb2c8e5db2026d309ae04d1777cb2ef654ae01df378`
- **Antrojo failo hash**:
- `eae735f220bddb58a613013e8c39a7a4e27f4d0a38353370be8b59564491bf7c`

### 3. Testas
Du vienodi failai, viename skiriasi vienas ženklas:

- **Pirmojo failo hash**:
- `4e5b9136cc395fa4ba57cdb2c8e5db2026d309ae04d1777cb2ef654ae01df378`
- **Antrojo failo hash**:
- `f346f671e1b4b4ef1f5292cded2030ab8b1e6ea9390c4c27d76aca05051828c3`

### 4. Testas
Tuščias failas:

- **Failo hash**: `644503e46283b90a40c1cf309e5f35c69c7d1b3c9a3b31c238f9876896176dbe`

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

Number of collisions: 743
Total unique hashes: 99014
Deja, gautas rezultatas nenudziugino, taciau galima pasitaisyti ir pabandyti dar karta. Kitas release bus su geresniu hashu generatoriumi


