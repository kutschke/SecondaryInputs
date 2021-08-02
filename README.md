# SecondaryInputs
Code to exercise the secondary input feature of art and test if it will work
as we need it to when we merge the file streams that come from Offline.

To exercise this:

setup mu2e
setup muse
muse link Offline                                           # This must work with any verison of art so use one used by the current Offline Musing
git clone https://github.com/kutschke/SecondaryInputs.git
muse build -j 8

## Step 1.
Run a job to make 4 art files: all.art trkcal.art crv_1.art crv_2.art

all.art contains 10 events with 3 data products each.  There is one data product to mock up the data from each of the trk, cal and crv.

trkcal.art contains the same 10 events but only the trk and cal data products

crv_1.art contains the only the crv data product but only for the first 5 events.
crv_2.art contains the only the crv data product but only for the last 5 events.



mu2e -c SecondaryInputs/fcl/all.fcl
mu2e -c SecondaryInputs/fcl/read.fcl -s all.art


mu2e -c SecondaryInputs/fcl/read.fcl -s crv_1.art
mu2e -c SecondaryInputs/fcl/read.fcl -s crv_2.art


mu2e -c SecondaryInputs/fcl/trkcal.fcl
mu2e -c SecondaryInputs/fcl/crv.fcl

mu2e -c SecondaryInputs/fcl/read.fcl -s trkcal_solo.art
mu2e -c SecondaryInputs/fcl/read.fcl -s crv_solo_1.art
mu2e -c SecondaryInputs/fcl/read.fcl -s crv_solo_2.art
mu2e -c SecondaryInputs/fcl/join_solo.fcl
