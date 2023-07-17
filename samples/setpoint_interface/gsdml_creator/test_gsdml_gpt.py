import difflib
import os

from assertpy import assert_that
from gsdml_gpt import GSDML


def test_generate_xml():
    # Create an instance of the GSDML class
    gsdml = GSDML()

    # Generate the XML output
    generated_xml = gsdml.root
    temp_fname = "generated.xml"
    gsdml.write_xml(temp_fname)

    with open(temp_fname, "r") as f:
        actual = f.readlines()

    with open("GSDML-V2.4-RT-Labs-unit-test-20230524.xml", "r") as f:
        expected = f.readlines()

    if actual != expected:
        print("\n".join(difflib.context_diff(actual, expected, n=1)))

    assert_that(actual).is_equal_to(expected)

    os.remove(temp_fname)
