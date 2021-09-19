# SecondaryInputs

The Mu2e TDAQ system will write files that are split as follows.  For all triggered events,
the Tracker (trk) and Calorimeter (cal) related data products will be written to one file
while the Cosmic Ray Veto (crv) related data prodcuts will be written to a separate file.
Only the trk and cal data are used to trigger events; the crv information is written
for all events that pass the trigger.  This organziation is driven by resource limitations
within the TDAQ system.

One of the first steps in processing the data will be to join the two files into one.
One option to do this is to use the art "secondary input file" feature to do this.
This repo contains some exercises that show that the basic funcationality works but that
there are issues that we need to work around:

1. The art job that *creates* the crv data products
   _**must**_ have the same art process_name as the art job that *creates* the trk and cal
   data products.
2. We expect that events written to the two files will not be sorted in order of
   increasing `art::EventID`; moreover the order may be different in the crv file
   than it is in the trk+cal file.  In this case the job that merges the two
   files into one must not set `noEventSort : true` in the configuration of the RootInput module;
   the default is `false`. This is not likely a serious problem.

To do exercise this you need to setup an appropriate version of art; it should
work with any recent version of art.  For example you can setup the
current Musing of Offline.

To establish the environment and build the code:

1. setup mu2e
2. setup muse
3. muse link Offline
4. git clone https://github.com/kutschke/SecondaryInputs.git
5. muse setup -1
6. muse build -j 8
7. mkdir \<path to your space on a data disk\>
8. ln -s \<path to your space on a data disk\> data

Instead of steps 7 and 8 you could just `mkdir data`.

## Exercise 1

Secondary input files are advertised to work if the secondary files "share a common history with the primary".
This first exercise does that explicitly. The second exercise will try the variant needed for merging files created by TDAQ.
To run the first step:

<pre>
   mu2e -c SecondaryInputs/fcl/all.fcl
</pre>

This job creates 3 toy data products, one to represent the trk data,
one for the cal data and one for the crv data.  Each data product is made by its own
module, with module labels maketrk, makecal and makecrv.  The toy data product is
a single integer with the structure:

<pre>
   10000*A + N
</pre>

where A is 1 for the tracker, 2 for the calorimeter and 3 for the crv; N is the event number.
This job makes 4 art files, all in the subdirectory data/:

| File | Content |
|------|---------|
| all.art    | events 1...10, with all of the trk, cal and crv toy data products |
| trkcal.art | events 1...10, with only the trk and cal data products |
| crv_1.art  | events 1...5, with only the crv data product|
| crv_2.art  | events 6...10, with only the crv data product|

You can examine that each file has the expected content with the following:

<pre>
   mu2e -c SecondaryInputs/fcl/read.fcl -s data/all.art
   mu2e -c SecondaryInputs/fcl/read.fcl -s data/trkcal.art
   mu2e -c SecondaryInputs/fcl/read.fcl -s data/crv_1.art
   mu2e -c SecondaryInputs/fcl/read.fcl -s data/crv_2.art
</pre>

The following job will read trkcal.art as the primary input
with crv_1.art and crv_2.art as secondary inputs:

<pre>
  mu2e -c SecondaryInputs/fcl/join.fcl
</pre>

You can verify that it produces the same output as running `read.fcl` on `all.art`.

This job also writes the merged output to the file `join.art`.
You can run `read.fcl` on this file and verify that it too makes the same output
as running `read.fcl` on `all.art`:

<pre>
   mu2e -c SecondaryInputs/fcl/read.fcl -s data/join.art
</pre>


# Exercise 2

This exercise mocks up what we will do in the TDAQ: one art job makes
the trk + cal file while a second art job makes the crv file.
A third art job merges the two files.
 Again all files are written to the subdirectory data/.
To start this exercise run two jobs:

<pre>
   mu2e -c SecondaryInputs/fcl/trkcal.fcl
   mu2e -c SecondaryInputs/fcl/crv.fcl
</pre>

Note that both jobs have the *same* art process_name; this is critical to making the example work.

The first job runs the trk and cal parts of all.fcl and writes those data products to a file `trkcal_tdaq.art`.
The second job runs the crv part of all.fcl and writes that data product to the files `crv_tdaq1_1.art` and `crv_tdaq1_2.art`;
again each of the crv files contains 5 events.

You can verify that these files have the expected content:

<pre>
   mu2e -c SecondaryInputs/fcl/read.fcl -s data/trkcal_tdaq.art
   mu2e -c SecondaryInputs/fcl/read.fcl -s data/crv_tdaq1_1.art
   mu2e -c SecondaryInputs/fcl/read.fcl -s data/crv_tdaq1_2.art
</pre>
To complete the exercise, run the following job to read `trkcal_tdaq.art` as the primary input file
and the two `crv_tdaq1*` files as the secondaries:

<pre>
   mu2e -c SecondaryInputs/fcl/join_tdaq1.fcl
</pre>
This makes the same output as running `read.fcl` on `all.art`.
It also writes the output file `join_tdaq1.art`.  You can run `read.fcl` on this to verify that it too is correct:

<pre>
   mu2e -c SecondaryInputs/fcl/read.fcl -s data/join_tdaq1.art
</pre>

So we have an existence proof that this will work for the minimal version of our use case.
We should add this test as part of the acceptance for a new art or ask that the art team do the equivalent.


## Exercise 3

This exercise, is almost identical to exercise 2, the difference being that the art process_name of `crv_tdaq2.fcl`
is different than that of `trkcal.fcl`.  It's files are distinguished from those of exercise 2 by the substitution
`tdaq1` with `tdaq2`.  The steps are:

<pre>
   mu2e -c SecondaryInputs/fcl/crv_tdaq2.fcl
</pre>
This makes the files, `crv_tdaq2_1.art` and `crv_tdaq2_2.art`. You can verify that these files have the expected content:

<pre>
   mu2e -c SecondaryInputs/fcl/read.fcl -s data/crv_tdaq2_1.art
   mu2e -c SecondaryInputs/fcl/read.fcl -s data/crv_tdaq2_2.art
</pre>
Next run the following job to read `trkcal_tdaq.art` as the primary input file
and the two `crv_tdaq2` files as the secondaries:

<pre>
   mu2e -c SecondaryInputs/fcl/join_tdaq2.fcl
</pre>

The printout from this job is correct; this surprised me because it is what
I understand is the advertised behaviour.

The job also writes the output file `join_tdaq2.art`.
If you look in `join_tdaq2.art` using a root TBrowser,
you will see that the data products are, in fact, there.

However, if you inspect this file using `read.fcl` you will see that the crv data products are missing:

<pre>
   mu2e -c SecondaryInputs/fcl/read.fcl -s data/join_tdaq2.art
</pre>

This is true even if you ask for them by specifying the process name in the input tag:

<pre>
   mu2e -c SecondaryInputs/fcl/read_crv_tdaq2.fcl -s data/join_tdaq2.art
</pre>

As a test that I did not blunder the fcl file, run it on one of the original crv files from
this exercise:

<pre>
   mu2e -c SecondaryInputs/fcl/read_crv_tdaq2.fcl -s data/crv_tdaq2_1.art
</pre>
This produces the expected output.

And as a second test, look for the crv data product using its full name in the files from
Exercise 2:
<pre>
   mu2e -c SecondaryInputs/fcl/read_crv_daq.fcl -s data/join_tdaq1.art
   mu2e -c SecondaryInputs/fcl/read_crv_daq.fcl -s data/crv_tdaq1_1.art
</pre>
These produce the expected output.


## Exercise 4

This exercise mocks up the next level of complexity: we expect that the events written to the
trk+cal file will not be written in order of increasing `art::EventID`.
The same for events written to the crv file; moreover the order within the crv file may
be different than the order in the trk+cal file.
For this exercise the process_name of job that creates the crv file has been set
to the same process name as was used for the trk+cal file;
this will be true all future exercises.

This exercise starts by creating new version of the trk+cal file
in which the events are ordered randomly.

<pre>
   mu2e -c SecondaryInputs/fcl/trkcal_split.fcl
   mu2e -c SecondaryInputs/fcl/gather.fcl -S SecondaryInputs/fcl/trkcal_gather.txt -o data/trkcal_gather.art
   mu2e -c SecondaryInputs/fcl/read_nosort.fcl -s data/trkcal_gather.art
   mu2e -c SecondaryInputs/fcl/read.fcl -s data/trkcal_gather.art
</pre>
The first job generates 20 output files, each containing one event.
The file SecondaryInputs/fcl/trkcal\_gather.txt lists all 20 files in a random order.
The second job reads the 20 files in the order specified by trkcal\_gather.txt and writes to the ouptut file specified on the command line.
The third job reads the newly created output file in the order found within art file.
The fourth job reads the newly created file the ususal way, with the events sorted in order increasing `art::EventID`.

Inspection of the output  will show that the expected data products are present and that events in trkcal\_gather.art are in the randomized order.

The next step is to do the same for the crv file:
<pre>
   mu2e -c SecondaryInputs/fcl/crv_split.fcl
   mu2e -c SecondaryInputs/fcl/gather.fcl -S SecondaryInputs/fcl/crv_gather.txt -o data/crv_gather.art
   mu2e -c SecondaryInputs/fcl/read_nosort.fcl -s data/crv_gather.art
   mu2e -c SecondaryInputs/fcl/read.fcl -s data/crv_gather.art
</pre>

Now the exercise proper can begin:
<pre>
   mu2e -c SecondaryInputs/fcl/join_gathered_sort.fcl
   mu2e -c SecondaryInputs/fcl/read_nosort.fcl data/split_gathered_joined_sort.art
</pre>
The first job joins data/trkcal\_gather.art and data/crv\_gather.art into data/split\_gathered\_joined\_sort.art.
This is the same as exercise 2, except that I did not split the crv file into two files.
Inspection of the output of these jobs will show that the expected data products are present and that the
events in the ouput file are written in order of increasing `art::EventID`.

The last part of the exercise is to see if it works if join job does not read the events in sorted order
from the primary input file:
<pre>
   mu2e -c SecondaryInputs/fcl/join_gathered_nosort.fcl
   mu2e -c SecondaryInputs/fcl/read_nosort.fcl data/split_gathered_joined_nosort.art
</pre>
Inspection of the output from the first job shows that, for most events,
art did not correctly find the crv data product; it only found it for event 7.


The conclusion is that we must not use `noEventSort : true` in the configuration of RootInput when
merging the two data files.  This is probably not a serious constraint because we probably don't
need to do this anyway.

## Exercise 5

This builds on  exercise 4 by making new copies of the \*\_gather.txt files
in which some events were removed
to mock up what happens when the trigger rejects events.
The same events were removed from both files.

<pre>
   mu2e -c SecondaryInputs/fcl/gather.fcl -S SecondaryInputs/fcl/trkcal_gather_1.txt -o data/trkcal_gather_1.art
   mu2e -c SecondaryInputs/fcl/gather.fcl -S SecondaryInputs/fcl/crv_gather_1.txt -o data/crv_gather_1.art
   mu2e -c SecondaryInputs/fcl/read_nosort.fcl data/trkcal_gather_1.art
   mu2e -c SecondaryInputs/fcl/read_nosort.fcl data/crv_gather_1.art
   mu2e -c SecondaryInputs/fcl/join_gathered_sort_1.fcl
   mu2e -c SecondaryInputs/fcl/join_gathered_nosort_1.fcl
   mu2e -c SecondaryInputs/fcl/read_nosort.fcl data/split_gathered_joined_sort_1.art
   mu2e -c SecondaryInputs/fcl/read_nosort.fcl data/split_gathered_joined_nosort_1.art
</pre>
This has a result similar to that of Exercise 4:
using the default `noEventSort : false` the merged output file is correct
but using `noEventSort : true` the output file does not contain crv information
for most events;
only event 8 has crv information, which a different event than in Exercise 4.

## Exercise 6
This builds on Exercise 5 by making a new file, crv\_gather\_2.txt, that removes 3 events
from crv\_gather\_1.txt; this exercise still uses trkcal\_gather\_1.txt.
This mocks up the TDAQ system missing some crv events even though
they triggered.

<pre>
   mu2e -c SecondaryInputs/fcl/gather.fcl -S SecondaryInputs/fcl/crv_gather_2.txt -o data/crv_gather_2.art
   mu2e -c SecondaryInputs/fcl/read_nosort.fcl data/crv_gather_2.art
   mu2e -c SecondaryInputs/fcl/join_gathered_sort_2.fcl
   mu2e -c SecondaryInputs/fcl/join_gathered_nosort_2.fcl
   mu2e -c SecondaryInputs/fcl/read_nosort.fcl data/split_gathered_joined_sort_2.art
   mu2e -c SecondaryInputs/fcl/read_nosort.fcl data/split_gathered_joined_nosort_2.art
</pre>
This has results similar to those of Exercises 4 and 5:
using the default `noEventSort : false` the merged output file is correct;
that is, if the crv information is available,
then it is present in the merged file
and, if it is not available,
then it is simply missing from the merged file and there are no errors reported.
However, using `noEventSort : true`, the output file does not contain crv information
for most events; this time it contains crv information for only events 8 and 19.

## Exercise 7
This builds on Exercise 6 by making a new file, trkcal\_gather\_3.txt, that removes 3 events
from trkcal\_gather\_1.txt; it still uses cal\_gather\_2.txt.
The result is that there will be some events in the crv file that are not present in
the trk+cal file.
This can happen if the crv file for offspill contains both triggered events
and the crv pedestal events, while the trk+cal file contains only triggered events.

In this example there all 4 classes of events: the event is present in both
the trk+cal file and the crv file; it is present in the trk+cal file but
not in the crv file; it is present in the crv file but not in the trk+cal file;
it is missing from both files.

<pre>
  mu2e -c SecondaryInputs/fcl/gather.fcl -S SecondaryInputs/fcl/trkcal_gather_3.txt -o data/trkcal_gather_3.art
  mu2e -c SecondaryInputs/fcl/read_nosort.fcl data/trkcal_gather_3.art
  mu2e -c SecondaryInputs/fcl/join_gathered_sort_3.fcl
  mu2e -c SecondaryInputs/fcl/join_gathered_nosort_3.fcl
  mu2e -c SecondaryInputs/fcl/read_nosort.fcl data/split_gathered_joined_sort_3.art
  mu2e -c SecondaryInputs/fcl/read_nosort.fcl data/split_gathered_joined_nosort_3.art
</pre>
This has results similar to that of Exercise 6:
using the default `noEventSort : false` the merged output file is correct
but , using `noEventSort : true`, the output file does not contain crv information
for most events; the events with crv information are the same as for Exercise 6.





