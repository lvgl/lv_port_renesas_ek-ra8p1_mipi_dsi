# lv_port_renesas_ek-ra8p1_mipi_dsi

SW4-6 ON - to enable MIPI DSI

Project uses srec-cat (https://srecord.sourceforge.net/) to do some post build manipulation of the data, 
to swap bytes in 16 bit word of the data that is programmed into the OSPI Flash

srec-cat is not commited to the git repo, it should be in the directory srecord/ in the project, the user should download it.

For more information see https://github.com/lvgl/lv_port_renesas_ek_ra8p1
