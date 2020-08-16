# Script for converting wav to mp3 using ffmpeg
# input files are in wavs subfolder

import os
from unittest import TestCase, main


class TestRent(TestCase):
    def setUp(self):
        self.rex = RenameExecutor()

    def testAssert(self):
        self.assertTrue(True)

    # check if you can get paths
    def testPath(self):
        ifl = os.listdir('wavs')
        ifn = ifl[1]
        self.assertTrue(ifn.startswith('Track'))

    # check if RenameExecutor can be created
    def testCanCreateRex(self):
        self.assertEqual(self.rex.input_location, 'wavs')
        self.assertEqual(self.rex.output_location, 'mp3s')

    # check if Rex can retrieve files
    def testCanRexGetInputFiles(self):
        f = self.rex.get_input_files()
        self.assertNotEqual(len(f), 0)
        for i in f:
            self.assertTrue(i.startswith(self.rex.input_location))

    # check if Rex can make output files
    def testCanRexNameOutputFiles(self):
        ifl = self.rex.get_input_files()
        ofl = self.rex.get_output_files()
        # print(len(ifl), len(ofl))
        # print(ifl)
        # print(ofl)
        self.assertNotEqual(len(ofl), 0)
        for ifn in ifl:
            self.assertTrue(
                ifn.replace(
                    self.rex.input_location,
                    self.rex.output_location
                ).replace(
                    self.rex.input_ext, self.rex.output_ext
                ) in ofl
            )

    # check if it returns commands
    def testCanRexMakeCommands(self):
        cmds = self.rex.get_commands()
        self.assertNotEqual(len(cmds), 0)
        print(cmds)


class RenameExecutor:
    def __init__(self):
        self.input_location = 'wavs'
        self.output_location = 'mp3s'
        self.cmd_format = 'ffmpeg -i "{}" "{}"'
        self.input_ext = 'wav'
        self.output_ext = 'mp3'

    def get_input_files(self):
        return [self.input_location + os.sep + x for x in os.listdir(self.input_location)]

    def get_output_files(self):
        return list(map(self.make_output_filename, self.get_input_files()))

    # Create output filename for corresponding input filename
    def make_output_filename(self, fn):
        return fn.replace(
            self.input_location, self.output_location
        ).replace(
            self.input_ext, self.output_ext
        )

    def get_commands(self):
        ifl = self.get_input_files()
        cmds = map(
            lambda x: self.cmd_format.format(
                x, self.make_output_filename(x)
            ),
            ifl
        )
        return list(cmds)


if __name__ == '__main__':
    # Delegate to unittest.main
    main()
