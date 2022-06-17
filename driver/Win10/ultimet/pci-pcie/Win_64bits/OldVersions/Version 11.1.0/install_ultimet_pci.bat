echo ****************************************
echo *** install ultimet_light PCI device ***
echo ****************************************

wdreg -inf ultimet_light.inf disable
wdreg -inf windrvr6.inf disable

wdreg -inf ultimet_light.inf uninstall
wdreg -inf windrvr6.inf uninstall

wdreg -inf windrvr6.inf install
wdreg -inf ultimet_light.inf install

pause
