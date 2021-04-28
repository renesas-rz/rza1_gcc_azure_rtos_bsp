import xml.etree.ElementTree as ET
import sys, os, json

START_OF_TEMPLATE = 46

DEBUG_ENABLED = True;

def Debug_Print(*args):
    if DEBUG_ENABLED:
        print(*args)


class code_generator:
    device = ""
    partNumber = ""
    pinCount = ""


    portPinTable = {}
    listOfPeripherials = []
    peripherialTable = {}
    peripherials = []
    gpio = []

    scPinConfigs = {}

    def __init__(self, filename):
        Debug_Print(filename)
        self.filename = filename


    def sc_parser(self):
        tree = ET.parse(self.filename)
        root = tree.getroot()

        # Get Board information
        self.device = root.find('Device').attrib['Family']
        self.partNumber = root.find('Device').attrib['PartNumber']
        self.pinCount = int(root.find('Device').attrib['PinCount'])

        # Determine the PinSource Filename
        if self.device == 'RZA1H' or self.device == 'RZA1M':
            pinSources = self.get_devicePinInfo('./scripts/rza1h/rza1h.xml')
        else:
            pinSources = self.get_devicePinInfo('./scripts/rza1l/rza1l.xml')

        for src in pinSources:
            if self.partNumber in src.keys():
                pSFileName = next(iter(src.values()))
                if self.device == 'RZA1H' or self.device == 'RZA1M':
                    pSFileName = "./scripts/rza1h/" + pSFileName
                else:
                    pSFileName = "./scripts/rza1l/" + pSFileName
                break
        """
        Get this target processor pin configurations
        """
        if pSFileName != None:
            self.portPinTable = self.buildTableOfPinFunctions(pSFileName)
        else:
            Debug_Print('Error No Pin Config file found for this ' + self.partNumber + ' Device')
            return

        """
        Get the Target Processor Peripheral info
        """
        if self.device == 'RZA1H' or self.device == 'RZA1M':
            self.listOfPeripherials = self.buildListOfPeripherials('./scripts/rza1h/peripheral_rza1h.xml')
            self.peripherialTable = self.buildTableOfPeripherialFunc('./scripts/rza1h/peripheral_rza1h.xml')
        else:
            self.listOfPeripherials = self.buildListOfPeripherials('./scripts/rza1l/peripheral_rza1l.xml')
            self.peripherialTable = self.buildTableOfPeripherialFunc('./scripts/rza1l/peripheral_rza1l.xml')

        """
        Parse the Smart Configurations Peripheral Settings
        """
        pins = root.find('Pin')
        self.scPinConfigs.update({ p.attrib['Id'] : p.attrib['Port']  for p in pins })
        # Configure on supported pins not fixed pins like USB
        self.scPinConfigs = {key: val for key, val in self.scPinConfigs.items() if key in self.peripherialTable}

        self.entryTable = self.createEntries()

    def sort_info(self):
        for p in self.scPinConfigs:
            if p['Id'] == p['Port']:
                # if the pin name is same as Port Pin name this is a GPIO
                self.gpio.append(p)
            else:
                # Otherwise this is a Periapherial Function Pin
                self.peripherials.append(p)

    def get_devicePinInfo(self, filename):
        pinSrc = []
        root = ET.parse(filename).getroot()
        for child in root:
            if 'pinInformation' in child.attrib['point']:
                pinSrc = [{c.attrib['device']: c.attrib['resource']} for c in child]

        return pinSrc

    def buildTableOfPinFunctions(self, filename):
        """
        Creates multiple tables each indexed by the port pin.
        Each table provides a ordered list of functions supported by that pin
        { PinName : [function1{}, function2{}, function3{}...]}
        """
        pin_configs = {}
        root = ET.parse(filename).getroot()
        for pin in root[1:]:
            if ('P' in pin[0].attrib['Id'] ):
                pin_configs.update({pin[0].attrib['Id']: [f.attrib for f in pin[1:]]})

        return pin_configs

    def buildTableOfPeripherialFunc(self, filename):
        """
        Creates the list of dictionaries. Each dictionary is Port
        """
        perInfo = {}
        root = ET.parse(filename).getroot()
        for Peripherals in root[1:]:
            # Check for sub Peripherials
            if Peripherals[0].tag == 'Pin':
                pass
                perInfo.update({func.attrib['Id']: Peripherals.attrib['Id'] for func in Peripherals if
                                Peripherals.attrib['Id'] in self.listOfPeripherials})
            else:
                for Peripheral in Peripherals:
                    perInfo.update({func.attrib['Id']: Peripheral.attrib['Id'] for func in Peripheral if
                                    Peripheral.attrib['Id'] in self.listOfPeripherials})

        return perInfo

    def buildListOfPeripherials(self, filename):
        peripherials = []
        perFil = ['USB.0', 'USB.1', 'USB.Common', 'GPIO', 'DEBUG', 'OMC', 'Power', 'NMI', 'EthernetAVB', 'DVD', 'SC',
                  'SG']

        root = ET.parse(filename).getroot()
        for perfs in root[1:]:
            if perfs[0].tag == 'Pin':
                peripherials.append(perfs.attrib['Id'])
            else:
                for channel in perfs:
                    peripherials.append(channel.attrib['Id'])
        # Apply the above filter
        peripherials = [p for p in peripherials if p not in perFil]
        # remove GPIO.n
        peripherials = [ p for p in peripherials if 'GPIO' not in p]

        return peripherials

    def createEntries(self):
        entryTable = { p: [] for p in self.listOfPeripherials }

        for func, pin in self.scPinConfigs.items():
            try :
                peripherial = self.peripherialTable[func]
            except :
                Debug_Print( func + "\t\tfunction not supported ");
                continue


            # iterate the function list for the pin in the portPinTable Table
            mode = 0
            pin_dir = 'None'
            if pin in self.portPinTable:
                # Loop through list of supported functions for this pin looking for the matching function
                for idx, f in enumerate(self.portPinTable[pin]):
                    if func == f['Id']:
                        mode = idx+1
                        if f['PinInputOutput'] == 'O':
                            pin_dir = 'OUTPUT'
                        elif f['PinInputOutput'] == 'I':
                            pin_dir = 'INPUT'
                        else:
                            pin_dir = 'IO'


                entryTable[peripherial].append({'Port': pin, 'Id': func, 'Alternate': str(mode), 'Direction': pin_dir})
            else:
                Debug_Print(pin + "\t\tPin Not supported")


        return entryTable
    def generateCode(self):
        self.fp = open('r_gpio_cfg.h', 'w')

        self.generateHeader()

        self.generatePeripherialEntry()

        self.generateFooter()

        self.fp.close()

    def generatePeripherialEntry(self):
        if self.fp != None:
            for peripherial in self.listOfPeripherials:
                device = "static st_port_config_t GPIO_SC_TABLE_" + peripherial.lower().replace(".", "") + "[] =\n{\n"
                self.fp.writelines(device)
                numberOfEntries = self.generateEntry(peripherial)
                self.fp.writelines('};\n')

                wrapper = "static const st_port_init_config_t GPIO_SC_INIT_" + peripherial.lower().replace(".", "") + " = { "
                self.fp.writelines(wrapper)
                wrapper = "0,\t" + str(numberOfEntries) + ',\t' + '&GPIO_SC_TABLE_' + peripherial.lower().replace(".", "") + '[0]'
                self.fp.writelines(wrapper)
                self.fp.writelines('\t};\n')

    def generateEntry (self, peripherial):
        """
        Creates a Peripherial entry
        """
        count = 0
        if self.fp != None :
            for func in self.entryTable[peripherial]:
                entry = "\t{" + func['Port'] + ',\t' + 'FUNCTION_MODE' + str(func['Alternate']) + ',\tPIN_' + func[
                    'Direction'] + '},\t// Pin Function : ' + func['Id'] + '\n'
                self.fp.writelines(entry)
                count += 1
        return count

    def generateHeader(self):

        tp = open('./scripts/header_template.h')

        # Added Renesas Header
        # Add macro dupicate definition checker
        for line in range(45):
            l = tp.readline()
            if line == 27:
                tp.readline()
                self.fp.writelines("#ifndef _R_GPIO_"+self.device+"_CFG_H_\n")
                self.fp.writelines("#define _R_GPIO_"+self.device+"_CFG_H_\n")
            else:
                self.fp.writelines(l)

        tp.close()
    def generateFooter(self):
        self.fp.writelines("\n")
        self.fp.writelines("#endif  /* _R_GPIO_"+self.device+"_CFG_H_ */\n")

    def generateMCUConfig(self):
        tp = open('./scripts/mcu_board_template.h')
        fp = open('mcu_board_select.h', 'w')

        for line in range(44):
            l = tp.readline()
            fp.writelines(l)

        l = "#define TARGET_RZA1             (TARGET_"+self.device+")\n"
        fp.writelines(l)
        if self.device == 'RZA1H' or self.device == 'RZA1M':
            l = "#define TARGET_BOARD            (TARGET_BOARD_RSK)"
        else:
            l = "#define TARGET_BOARD            (TARGET_BOARD_STREAM_IT2)"
        fp.writelines(l)
        fp.writelines("\n\r")
        fp.writelines("#endif /* MCU_SEL_H */")

# Press the green button in the gutter to run the script.
if __name__ == '__main__':
    rz = code_generator(sys.argv[1])
    rz.sc_parser()
    Debug_Print('Device:\t',rz.device)
    Debug_Print('PartNumber: ', rz.partNumber)

    Debug_Print("portPinTable :\t",        rz.portPinTable)
    Debug_Print("peripherialTable :\t",    rz.peripherialTable)
    Debug_Print("listOfPeripherials :\t",  rz.listOfPeripherials)
    Debug_Print("scPinConfigs :\t",        rz.scPinConfigs)

    rz.generateCode()
    rz.generateMCUConfig()

# See PyCharm help at https://www.jetbrains.com/help/pycharm/
