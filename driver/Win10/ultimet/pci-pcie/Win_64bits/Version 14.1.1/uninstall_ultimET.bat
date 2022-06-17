echo ******************************************
echo *** uninstall ultimet_light PCI device ***
echo ******************************************

wdreg -inf ultimET.inf disable
wdreg -inf windrvr1411.inf disable

wdreg -inf ultimET.inf uninstall
wdreg -inf windrvr1411.inf uninstall

pause
