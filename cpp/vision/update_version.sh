if [[ $# -lt 1 ]];then
    echo [FATAL]: expecting update_version.sh SolutionDir 
    exit
fi

SOLUTION_DIR=$1
VERSION_BASE_FILE=version_base.h
VERSION_FILE=version.h
WCREV=`svn info $SOLUTION_DIR | grep Revision | awk '{print $2}'`
LOCAL_MODIFIED_COUNT=`svn status $SOLUTION_DIR | awk '{print $1}' | grep "M" | wc -l`
WCMODS=`echo $(($LOCAL_MODIFIED_COUNT>0?1:0))`
WCDATE=`svn info | grep 'Last Changed Date'| sed 's/.*\([0-9]\{4\}-[0-9]\{1,2\}-[0-9]\{1,2\}\s\+[0-9]\{1,2\}:[0-9]\{1,2\}:[0-9]\{1,2\}\).*/\1/'`
WCNOW=`date '+%Y-%m-%d %H:%M:%S'`

# echo $WCREV
# echo $LOCAL_MODIFIED_COUNT
# echo $WCMODS
# echo $WCDATE
# echo $WCNOW

# force create new version temp file
TEMP_FILENAME=$SOLUTION_DIR/NFComm/NFPluginLoader/$VERSION_BASE_FILE"_temp"
if [ -f "$TEMP_FILENAME" ]; then
    rm -f $TEMP_FILENAME
fi

cp $SOLUTION_DIR/NFComm/Utility/$VERSION_BASE_FILE $TEMP_FILENAME

# replace version number
REPLACE_CONTENT=`sed -i 's/\$WCREV\\$/'"$WCREV"'/g' $TEMP_FILENAME`

# repalce whether has file local modified
if [ $LOCAL_MODIFIED_COUNT -gt 0 ]; then
    REPLACE_CONTENT=`sed -i 's/\$WCMODS?true:false\\$/true/g' $TEMP_FILENAME`
else
    REPLACE_CONTENT=`sed -i 's/\$WCMODS?true:false\\$/false/g' $TEMP_FILENAME`
fi

# replace the lastest commited date
REPLACE_CONTENT=`sed -i 's/\$WCDATE\\$/'"$WCDATE"'/g' $TEMP_FILENAME`

# replace the build time
REPLACE_CONTENT=`sed -i 's/\$WCNOW\\$/'"$WCNOW"'/g' $TEMP_FILENAME`

mv $TEMP_FILENAME $SOLUTION_DIR/NFComm/NFPluginLoader/$VERSION_FILE
