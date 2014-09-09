#/bin/bash

echo
echo " * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *"
echo " *                                                           *"
echo " *>./copy_file.sh run_number (example: >./copy_file.sh 2910) *"
echo " *                                                           *"
echo " * CAUTION : PLEASE HIT yes TWICE!!!                         *"
echo " *                                                           *"
echo " * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *"
echo

scp edm@eddad1:~/Online/EddaAna/autosave/Run$1.root ./data/

