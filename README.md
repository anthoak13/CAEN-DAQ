# CAEN-DAQ
A ROOT based GUI for CAEN digitizers and linuxx8664gcc architectures

##Prerequisites
  - ROOT either v5 or v6
  - g++, at least 4.7.x
  - Wavedump (to use data aquision capabilities)
  
##Building
  1. Checkout the git repo and build source
    
    ```
    $ git clone https://github.com/anthoak13/CAEN-DAQ.git
    $ cd CAEN-DAQ
    $ make
    ```
  2. Run either as interpreted macro or compile GUI
    - Interpreted: `. ./run.sh`
      
    - Compile (Need to add provided libraries to the LD_LIBRARY_PATH enviroment variable)
    
     ```
     $ make build
     $ LD_LIBRARY_PATH=$LD_LIBRARY_PATH:`pwd`/lib
     $ ./gui.out
     ```
  3. Directory can be cleaned with `$ make clean`
 
##Updating
The repo can be brought to the most recent version with 
```
git fetch origin master
git reset --hard FETCH_HEAD
```
The source will have to be rebuilt
