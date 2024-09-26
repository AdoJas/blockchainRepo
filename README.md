# blockchainRepo

# Repozitorija, skirta blockchain modulio darbams

## Testavimas

### 1. Testas
Du failai sudaryti iš vieno, bet skirtingo simbolio:

- **Failo 1**: `aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa`  
  Hash: `2b2f9929dfbbf9590b075979a76bf959dbff89794f8b2969db67a949079b4949`

- **Failo 2**: `bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb`  
  Hash: `41b11f8f31e17beb5191c7a77131cbdb81116f3f11a10b1b51b187a7f1111beb`

### 2. Testas
Du failai iš atsitiktinai sugeneruotų simbolių:

- **Pirmojo failo hash**: `4e5b9136cc395fa4ba57cdb2c8e5db2026d309ae04d1777cb2ef654ae01df378`
- **Antrojo failo hash**: `eae735f220bddb58a613013e8c39a7a4e27f4d0a38353370be8b59564491bf7c`

### 3. Testas
Du vienodi failai, viename skiriasi vienas ženklas:

- **Pirmojo failo hash**: `4e5b9136cc395fa4ba57cdb2c8e5db2026d309ae04d1777cb2ef654ae01df378`
- **Antrojo failo hash**: `f346f671e1b4b4ef1f5292cded2030ab8b1e6ea9390c4c27d76aca05051828c3`

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

![img.png](img.png)

### 6. Testas
**Koliziju testas**

Number of collisions: 743
Total unique hashes: 99014
Deja, gautas rezultatas nenudziugino, taciau galima pasitaisyti ir pabandyti dar karta. Kitas release bus su geresniu hashu generatoriumi


