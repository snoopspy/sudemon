#!/system/bin/sh
echo "Running sudemon in the backgound..."
nohup sudemon >> /data/local/tmp/sudemon.log 2>&1 &
#nohup sudemon > /dev/null 2>&1 &
