#program to parse threat file and printout those
# that reference gitlab
#
import xml.etree.ElementTree as ET

tree = ET.parse('allitems-cvrf-year-2022.xml')
root = tree.getroot()

print(root.tag)


def print_references(references):
    if references != None:
        for reference in references:
            for bits in reference:
                tag = ""
                if 'URL' in bits.tag:
                    tag = 'URL'
                if 'Description' in bits.tag:
                    tag = 'Description'
                print("    ", tag, ":",  bits.text)

def print_notes(notes):
    if notes != None:
        for note in notes:
            title = note.attrib['Title']+":" if note.attrib['Type'] == "Other" else ""
            print("    (", note.attrib['Ordinal'], "):", title, note.text)

count = 0
for child in root:
    if 'Vulnerability' in child.tag:
        printit = False
        #print(type(child.attrib), child.attrib)
        vulnerability_dict = child.attrib
        vnumber = vulnerability_dict['Ordinal']
        references = None
        for gkid in child:
            #print(type(gkid), gkid.tag, gkid.attrib, gkid.text)
            if 'Title' in gkid.tag:
                title = gkid.text
            if 'References' in gkid.tag:
                references = gkid
            if 'Notes' in gkid.tag:
                notes = gkid
        if references != None:
            #print(references)
            for reference in references:
                #print("reftag:", reference.tag,
                #      "- refattrib:", reference.attrib,
                #      "- reftext:", reference.text, "-")
                for bits in reference:
                    if 'gitlab' in bits.text:
                    #    print(bits.text)
                        printit = True

                #if 'gitlab' in url.text or 'gitlab' in description.text:
                #    printit = True
        if printit:        
            print("Vulnerability Nubmer: ", vulnerability_dict['Ordinal'])
            print("Title : ", title)
            print("Notes:")
            print_notes(notes)
            print("Rererences:")
            print_references(references)
            print()
            count +=1

#print(count)

