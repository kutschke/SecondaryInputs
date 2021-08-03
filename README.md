# SecondaryInputs
Code to exercise the secondary input feature of art and test if it will work
as we need it to when we merge the file streams that come from Offline.

To exercise this you need to setup an appropriate version of art.
You can do this by using the current Musing of Offline.
This should work with any version of art.  If it does not we need to fix it.

To establish the environment and build the code:

1. setup mu2e
2. setup muse
3. muse link Offline
4. git clone https://github.com/kutschke/SecondaryInputs.git
5. muse setup -1
5. muse build -j 8

## Example 1.

Secondary input files are only advertised to work if the secondary files "share a common history with the primary".
This is the example that is advertised by art to work.

> `mu2e -c SecondaryInputs/fcl/all.fcl`

This job creates 3 toy data products, one to represent the trk data,
one for the cal data and one for the crv data.  Each is made by it's own
module labels maketrk, makecal and makecrv.  The toy data product is a
a single integer with the structure:

> `10000*A + N`

where A is 1 for the tracker, 2 for the calorimeter and 3 for the crv.

This job makes 4 art files:   all.art trkcal.art crv_1.art crv_2.art

| File | Content |
|------|---------|
| all.art    | events 1...10, with all of the trk, cal and crv toy data products |
| trkcal.art | events 1...10, with only the trk and cal data products |
| crv_1.art  | events 1...5, with only the crv data product|
| crv_2.art  | events 6...10, with only the crv data product|



mu2e -c SecondaryInputs/fcl/read.fcl -s all.art


mu2e -c SecondaryInputs/fcl/read.fcl -s crv_1.art
mu2e -c SecondaryInputs/fcl/read.fcl -s crv_2.art


mu2e -c SecondaryInputs/fcl/trkcal.fcl
mu2e -c SecondaryInputs/fcl/crv.fcl

mu2e -c SecondaryInputs/fcl/read.fcl -s trkcal_solo.art
mu2e -c SecondaryInputs/fcl/read.fcl -s crv_solo_1.art
mu2e -c SecondaryInputs/fcl/read.fcl -s crv_solo_2.art
mu2e -c SecondaryInputs/fcl/join_solo.fcl
