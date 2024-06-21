This folder is part of our test system and does the followings things
 - executes print.du with the latest donsus build(1)
 - writes the output into a file(2)
 - then compares this output(2) with print_expected.txt
 - if the transformation is correct the test passes, if not it fails

Note that, there is only one test suite responsible for running these test files.
It is not seperated like others, e.g in parser or typecheck.


To add new test cases: write the donsus file(.du) with a corresponding "..._expected.txt".

If the code is supposed return the error use "error" in the filename.