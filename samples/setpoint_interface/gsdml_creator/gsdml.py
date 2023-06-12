import xml.dom.minidom
import xml.etree.ElementTree as ET
from datetime import datetime
from enum import Enum
from typing import Optional

from hat.ddd import BaseModel


class DataType(Enum):
    BOOLEAN = "Boolean"
    INTEGER8 = "Integer8"
    INTEGER16 = "Integer16"
    INTEGER32 = "Integer32"
    INTEGER64 = "Integer64"
    UNSIGNED8 = "Unsigned8"
    UNSIGNED16 = "Unsigned16"
    UNSIGNED32 = "Unsigned32"
    UNSIGNED64 = "Unsigned64"
    FLOAT32 = "Float32"
    FLOAT64 = "Float64"

    def __str__(self):
        return self.value


class Setpoint(BaseModel):
    name: str
    unit: Optional[str] = None


class Gsdml:
    def __init__(self, root_name, name):
        self.root = ET.Element(root_name)
        self.name = name
        self.date = datetime.now().strftime("%Y%m%d")
        self.input_consistency = ET.SubElement(self.root, "Input")
        self.input_consistency.set("Consistency", "All items consistency")
        self.primary_language = ET.SubElement(self.root, "PrimaryLanguage")

    def add_input_data_item(self, data_type: DataType, text_id):
        data_item = ET.SubElement(self.input_consistency, "DataItem")
        data_item.set("DataType", str(data_type))
        data_item.set("TextId", text_id)

    def add_text_item(self, text_id, name):
        text_item = ET.SubElement(self.primary_language, "Text")
        text_item.set("TextId", text_id)
        text_item.set("Value", name)

    def save_to_file(self):
        filename = f"GSDML-V2.4-{self.name}-{self.date}.xml"
        xml_string = ET.tostring(self.root, encoding="utf-8", method="xml")

        # Create a new minidom object
        dom_string = xml.dom.minidom.parseString(xml_string)
        # Write the prettified XML to file
        with open(filename, "w") as file:
            file.write(dom_string.toprettyxml(indent="  ", newl="\n", encoding="utf-8").decode("utf-8"))
