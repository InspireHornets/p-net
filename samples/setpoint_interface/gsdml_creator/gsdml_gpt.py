import xml.etree.ElementTree as ET


class GSDML:
    @staticmethod
    def create_profile_header():
        """
        ProfileHeader definition as defined in ISO 15745-1.
        Please do not change the content.
        """
        profile_header = ET.Element("ProfileHeader")

        profile_identification = ET.SubElement(profile_header, "ProfileIdentification")
        profile_identification.text = "PROFINET Device Profile"

        profile_revision = ET.SubElement(profile_header, "ProfileRevision")
        profile_revision.text = "1.00"

        profile_name = ET.SubElement(profile_header, "ProfileName")
        profile_name.text = "Device Profile for PROFINET Devices"

        profile_source = ET.SubElement(profile_header, "ProfileSource")
        profile_source.text = "PROFIBUS Nutzerorganisation e. V. (PNO)"

        profile_class_id = ET.SubElement(profile_header, "ProfileClassID")
        profile_class_id.text = "Device"

        iso15745_reference = ET.SubElement(profile_header, "ISO15745Reference")

        iso15745_part = ET.SubElement(iso15745_reference, "ISO15745Part")
        iso15745_part.text = "4"

        iso15745_edition = ET.SubElement(iso15745_reference, "ISO15745Edition")
        iso15745_edition.text = "1"

        profile_technology = ET.SubElement(iso15745_reference, "ProfileTechnology")
        profile_technology.text = "GSDML"

        return profile_header

    def __init__(self):
        self.root = ET.Element("ISO15745Profile")
        self.root.set("xmlns", "http://www.profibus.com/GSDML/2003/11/DeviceProfile")
        self.root.set("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance")
        self.root.set(
            "xsi:schemaLocation",
            r"http://www.profibus.com/GSDML/2003/11/DeviceProfile ..\xsd\GSDML-DeviceProfile-V2.4.xsd",
        )

        self.profile_header = self.create_profile_header()
        self.root.append(self.profile_header)

    def write_xml(self, file_path):
        tree = ET.ElementTree(self.root)
        ET.indent(tree, "   ")
        tree.write(
            file_path,
            encoding="iso-8859-1",
            xml_declaration=True,
        )
