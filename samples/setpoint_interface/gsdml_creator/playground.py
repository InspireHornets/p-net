from gsdml import DataType, Gsdml, Setpoint

setpoints = [
    Setpoint(name="Id"),
    Setpoint(name="Position", unit="um"),
    Setpoint(name="Speed", unit="mm/min"),
    Setpoint(name="Acceleration", unit="mm/min2"),
]

gsdml = Gsdml("Root", "inspire-ag-setpoint-interface")
for i in range(1, 11):
    for setpoint in setpoints:
        text_id = f"Input_Setpoint_{i}_{setpoint.name}"
        if setpoint.unit:
            text_id += f"_{setpoint.unit.replace('/', '_')}"
        gsdml.add_input_data_item(data_type=DataType.INTEGER32, text_id=text_id)
        gsdml.add_text_item(text_id=text_id, name=f"{setpoint.name} {i} x ({setpoint.unit})")

gsdml.save_to_file()
