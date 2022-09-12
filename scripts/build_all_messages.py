import os, subprocess

# get all message files
messageFiles = []
for root, dirs, files in os.walk(os.getcwd() + "/../messages"):
    for name in files:
        if name.endswith('.idl'):
            messagePath = os.path.join(root, name)
            messageDir = os.path.split(messagePath)[0]
            messageFiles.append([messageDir, messagePath])

# run the fastdds message generation script for each message .idl file (assuming dds directory is in home)
ddsLocation = "~/"
for messageFile in messageFiles:
    subprocess.run(["dds/src/fastddsgen/scripts/fastddsgen", messageFile[1], "-d", messageFile[0]], cwd=os.path.expanduser(ddsLocation))
    


