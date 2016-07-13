# CAEN-DAQ
A ROOT based GUI for CAEN digitizers and linuxx8664gcc architectures

##Prerequisites
  - [ROOT](https://root.cern.ch/) either v5 or v6
  - gcc 4.7.x or above (installation directions [here](http://luiarthur.github.io/gccinstall))
  - [Wavedump](http://www.caen.it/csite/CaenProd.jsp?parent=38&idmod=692) (to use data aquision capabilities)
  
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

##License and disclaimer

CAEN-DAQ licensed under the GNU General Public License v3.0. You have the right to modify and/or redistribute this source code under the terms specified in the license,

CAEN-DAQ is provided without any warranty nor guarantee of fitness for any particular purpose. The author(s) shall have no liability for direct, indirect, or other undesirable consequences of any character that may result from the use of this source code. This may include - but is not limited - to irrevocable changes to the user's firmware, software, hardware, or data. By your use of CAEN-DAQ, you implicitly agree to absolve the author(s) of any liability whatsoever.

The CAEN libraries - which are distributed (a) for the user's convenience and (b) to ensure version compatibility - are also licensed under the GNU General Public License v3.0 by CAEN S.p.A.

A copy of the GNU General Public License v3.0 may be found within this repository at License.md or is available online at http://www.gnu.org/licenses.
