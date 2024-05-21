#######################################################################################################################################
#######################################################################################################################################
#################               This script was created by github.com/MarkCyber (w/ assistance of ai)              ####################         
#################       This is a python script to automatically create BadUSB scripts to auto send emails         ####################
#################              This takes a excel sheet with the columns named "Names" and "Emails"                ####################
#################        This script will then make a badusb script using the name + email of each person          ####################
#################          There are various subject options that will be chosen from, to minimize "spam"          ####################
#################          Change the signature to your name, and put subject options that fit your email          ####################
#################         Lastly, of course make sure to change the contents of the email to what you want         ####################
#######################################################################################################################################
#######################################################################################################################################

import pandas as pd
import random

# Load the Excel file, make sure it has the same name (or change the name in this script)
file_path = 'NameAndEmails.xlsx'
data_df = pd.read_excel(file_path)

# Your excel should have 2 columns. Names, and Emails. 
data_cleaned_df = data_df[['Names', 'Emails']].dropna().reset_index(drop=True)
data_cleaned_df.columns = ['Name', 'Email']

# List of placeholder subject options. Change these to 7 similar subjects that match your email (if you are sending many. You can use the same if not)
subject_options = [
    "Placeholder for subject option 1",
    "Placeholder for subject option 2",
    "Placeholder for subject option 3",
    "Placeholder for subject option 4",
    "Placeholder for subject option 5",
    "Placeholder for subject option 6",
    "Placeholder for subject option 7"
]

# Placeholder for email body template. The name field will be filled from the "names" section in the excel sheet you provided.
# Just modify the actual email body and sender name to fit your needs
email_body_template = """
Hi {name},

Placeholder for email body.

Warm Regards,
Sender Name
"""

# Function to generate BadUSB script
def generate_badusb_script_with_placeholders_single_file(data_df):
    script_template = [
        "DELAY 1000",
        "GUI r",
        "DELAY 500",
        "STRING chrome",
        "ENTER",
        "DELAY 1000",
        "STRING https://mail.google.com/mail/u/0/#inbox?compose=new", # In chrome it opens gmail to compose an email. This is why you must be logged in.
        "ENTER",
        "DELAY 5000"
    ]
    
    scripts = script_template
    
    for index, row in data_df.iterrows():
        name = row['Name']
        email = row['Email']
        subject = random.choice(subject_options)
        random_delay = random.randint(10000, 25000)
        
        email_body_lines = email_body_template.format(name=name).strip().split('\n')
        email_body_lines = [f"STRING {line.strip()}" for line in email_body_lines if line.strip()]

        email_script = [
            "DELAY 1000",
            f"STRING {email}",
            "DELAY 500",
            "TAB",
            "TAB",
            f"STRING {subject}",
            "TAB"
        ] + email_body_lines + [
            "ENTER",
            "CONTROL ENTER",
            "DELAY 5000",
            f"DELAY {random_delay}", #random delay so emails are not sent at the sames, ideally minimizing the potential to be marked as spam
            "ALT F4"
        ]

        scripts += email_script
    
    return "\n".join(scripts)

# Generate the BadUSB script with placeholders and proper send command in a single file
final_script_with_placeholders = generate_badusb_script_with_placeholders_single_file(data_cleaned_df)

# Save the script to a file
final_script_file_path = 'final_script_with_placeholders.txt' #This would be your badusb script
with open(final_script_file_path, 'w') as file:
    file.write(final_script_with_placeholders)

print(f"Script saved to {final_script_file_path}")

#check out github.com/markcyber for more badusb / pen testing / automation tools and scripts
