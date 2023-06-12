import os
from datetime import datetime

from assertpy import assert_that
from gsdml import DataType, Gsdml


def test_add_input_data_item():
    name = "MyTest"
    gsdml_doc = Gsdml("Root", name)
    gsdml_doc.add_input_data_item(DataType.INTEGER8, "ID1")

    assert_that(len(gsdml_doc.input_consistency)).is_equal_to(1)
    assert_that(gsdml_doc.input_consistency[0].tag).is_equal_to("DataItem")
    assert_that(gsdml_doc.input_consistency[0].attrib["DataType"]).is_equal_to("Integer8")
    assert_that(gsdml_doc.input_consistency[0].attrib["TextId"]).is_equal_to("ID1")


def test_add_text_item():
    name = "MyTest"
    gsdml_doc = Gsdml("Root", name)
    gsdml_doc.add_text_item("ID1", "SampleValue")

    assert_that(len(gsdml_doc.root)).is_equal_to(2)
    assert_that(gsdml_doc.primary_language[0].tag).is_equal_to("Text")
    assert_that(gsdml_doc.primary_language[0].attrib["TextId"]).is_equal_to("ID1")
    assert_that(gsdml_doc.primary_language[0].attrib["Value"]).is_equal_to("SampleValue")


def test_save_to_file():
    name = "MyTest"
    gsdml_doc = Gsdml("Root", name)
    gsdml_doc.add_input_data_item(DataType.INTEGER8, "ID1")
    gsdml_doc.save_to_file()

    date = datetime.now().strftime("%Y%m%d")
    expected_filename = f"GSDML-V2.4-{name}-{date}.xml"
    assert_that(os.path.isfile(expected_filename)).is_true()

    os.remove(expected_filename)  # Clean up after test
