import json
from pathlib import Path

TEMPLATE_DIR = Path(".")
OUTPUT_DIR = Path(".")

# Make sure output directory exists
OUTPUT_DIR.mkdir(exist_ok = True)

# Load data from JSON
with open("data.json", "r", encoding="utf-8") as f:
    pages = json.load(f)

# Read template files
template_c = (TEMPLATE_DIR / "menu_page_template.c").read_text()
#template_h = (TEMPLATE_DIR / "menu_page_template.h").read_text()

for page in pages:
    menu_type   = page["menu_type"]
    name        = page["page_name"]
    page_type   = page["page_type"]
    title       = page["page_title"]
    return_page = page["return_page"]
    filename    = page_type.lower() + "_" + name.lower()

    replacements = {
        "{{PAGE_NAME}}": name,
        "{{PAGE_TYPE}}": page_type,
        "{{PAGE_NAME_UPPER}}": page_type.upper() + "_" + name.upper(),
        "{{MENU_TYPE}}": menu_type,
        "{{FILE_NAME}}": page_type.lower() + "_" + name.lower(),
        "{{RETURN_PAGE}}": return_page,
    }

    # Apply replacements
    c_code = template_c
    #h_code = template_h
    for key, val in replacements.items():
        c_code = c_code.replace(key, val)
        #h_code = h_code.replace(key, val)

    # Write new files
    (OUTPUT_DIR / f"{filename}.c").write_text(c_code)
    #(OUTPUT_DIR / f"{name}.h").write_text(h_code)

    print(f"Generated: {filename}.c")
    #print(f"Generated: {name}.h")

print("✅ All menu files generated!")
