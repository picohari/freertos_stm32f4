import json
from pathlib import Path

TEMPLATE_DIR = Path(".")
OUTPUT_DIR = Path("../pages")

# Make sure output directory exists
OUTPUT_DIR.mkdir(exist_ok = True)

# Load data from JSON
with open("data.json", "r", encoding="utf-8") as f:
    pages = json.load(f)

# Read template files
template_c = (TEMPLATE_DIR / "menu_page_template.c").read_text()
#template_h = (TEMPLATE_DIR / "menu_page_template.h").read_text()

for page in pages:
    name = page["page_name"]
    title = page["page_title"]

    replacements = {
        "{{PAGE_NAME}}": name,
        "{{PAGE_NAME_UPPER}}": name.upper(),
        "{{PAGE_TITLE}}": title,
    }

    # Apply replacements
    c_code = template_c
    #h_code = template_h
    for key, val in replacements.items():
        c_code = c_code.replace(key, val)
        #h_code = h_code.replace(key, val)

    # Write new files
    (OUTPUT_DIR / f"{name}.c").write_text(c_code)
    #(OUTPUT_DIR / f"{name}.h").write_text(h_code)

    print(f"Generated: {name}.c")
    #print(f"Generated: {name}.h")

print("✅ All menu files generated!")
