# MB89623R Sub-MCU I/O Pin Usage

|Pin |MCU Name |Board Name   |In/Out |Description|
|----|---------|-------------|-------|-----------|
|48  |P00/AD0  |UPD_DATA     |I/O    |uPD16432B data |
|47  |P01/AD1  |UPD_CLK      |O      |uPD16532B clock |
|46  |P02/AD2  |UPD_STB      |O      |uPD16432B strobe |
|45  |P03/AD3  |UPD_OE       |O      |uPD16432B output enable (turns off LCD) |
|44  |P04/AD4  |DIAG_SW1     |I      | |
|43  |P05/AD5  |DIAG_SW2     |I      | |
|42  |P06/AD6  |BOSE_SW1     |I      | |
|41  |P07/AD7  |BOSE_SW2     |I      | |
|32  |P20/BUFC |/S2M_ENA_OUT |O      |Sub-to-Main SPI enable output|
|50  |P30/ADST |/M2S_ENA_IN  |I      |  Main-to-Sub SPI enable input |
|51  |P31/SCK1 |M2S_CLK_IN   |I      |  Main-to-Sub SPI clock input |
|53  |P33/SI1  |M2S_DATA_IN  |I      |  Main-to-Sub SPI data input |
|54  |P34/EC   |/S2M_CLK_OUT |O      |  Sub-to-Main SPI clock output |
|55  |P35/PWC  |S2M_DATA_OUT |O      |  Sub-to-Main SPI data output |
|57  |P36/WTO  |ILL_CONT1    |O      |  Illumination control terminal |
|58  |P37/PTO  |ILL_CONT2    |O      |  .. |
|62  | P43     |/DISP_ENA_OUT|O      |3LB enable output |
|64  | P45/SCK2|/DISP_CLK    |O      |3LB clock output |
|1   | P46/SO2 |DISP_DATA    |O      |3LB data output |
|2   | P47/SI2 |DISP_ENA_IN  |I      |3LB enable input |
|5   |P52/AN2  |DIAG SP FL AD|I      |Used for diagnosis by self-diagnostic speaker |
|6   |P53/AN3  |DIAG SP FR AD|I      |.. |
|7   |P54/AN4  |DIAG SP RL AD|I      |.. |
|8   |P55/AN5  |DIAG SP RR AD|I      |.. |
|9   |P56/AN6  |BACK UP AD   |I      |.. |
|10  |P57/AN7  |CLIP AD      |I      |Used for clip detection |
|14  |P60/INT0 |MFSW         |I      |MFSW input |
|15  |P61/INT1 |/UPD_KEY_REQ |I      |Key request input terminal from uPD16432B |
|16  |P62/INT2 |CLIPPING     |I      |Clip signal input terminal from POWER IC. |
|17  |P63/INT3 |DIAG ILL     |I      |Illumination pulse detection input terminal |
