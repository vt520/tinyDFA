#!/bin/bash
WORKING_FOLDER="${PWD}"
LIB_NAME=$(basename "${WORKING_FOLDER}")
RELEASE=${RELEASE:-false}

BUILD_BASE="$(mktemp -d --suffix=${LIB_NAME})"
LOG_FILE="${WORKING_FOLDER}/.build.log"
CONFIGURATION="${WORKING_FOLDER}/library.properties"
get_config() {
    grep -E "^${1}=" "${CONFIGURATION}" | tail -n 1 | sed -r "s/^[^=]+=//"
}
set_config() {
    grep -E "^${1}=" "${CONFIGURATION}" &> /dev/null && {
        sed -ri "s/^(${1})=.*/\1=${2}/" "${CONFIGURATION}"
    } || {
        echo "${1}=${2}" >> "${CONFIGURATION}"
    }
}
log_command() {
    echo "< $@ >" >> "${LOG_FILE}"
    $@ >> "${LOG_FILE}"
}
tee_command() {
    echo "< $@ >" >> "${LOG_FILE}"
    $@ | tee -a "${LOG_FILE}"
}
output() {
    echo $@ | tee -a "${LOG_FILE}"
}
break_version() {
    echo $1 | sed -r "s/\./ /g"
}
major_v() {
    echo $1
}
minor_v() {
    echo $2
}

BUILD_DESTINATION="${BUILD_BASE}/${LIB_NAME}/"

[ -e "library.properties" ] || {
    output "library.properties" missing are you in a library folder 
    exit    
}


output "Getting Build Information"
[ -e library.build ] || echo 0 > library.build
echo $(( 1 + $(<library.build))) > library.build
BUILD=$(<library.build)
VERSION_VALUE=$(get_config version) 
VERSION_ID=$(break_version "$VERSION_VALUE")
MAJOR_V=$(major_v $VERSION_ID)
MINOR_V=$(minor_v $VERSION_ID)
VERSION_VALUE="$MAJOR_V.$MINOR_V.$BUILD"
set_config version "$VERSION_VALUE"

EXPORT_FOLDER="${WORKING_FOLDER}/release/${VERSION_VALUE}/"
output Creating Release folder
mkdir -p "${EXPORT_FOLDER}"

output Snapshotting Release Source
log_command tar cvf \
    "${EXPORT_FOLDER}/${LIB_NAME}_${VERSION_VALUE}.tar" \
    $(find . -type f \
        -not -iname "*.tar" \
        -not -iname "*.zip" \
        -not -wholename "./build/*" \
        -not -wholename "./.*" -not \
        -wholename "./release/*")

output Building ${LIB_NAME}-$VERSION_VALUE Library in ${BUILD_DESTINATION}
log_command mkdir -p "${BUILD_DESTINATION}src/${LIB_NAME}" 

output Including root files
log_command cp -rpv \
    "examples" \
    "keywords.txt" \
    "library.properties" \
    "LICENSE" \
    "README.md" \
    "${BUILD_DESTINATION}"

output Including library source 
log_command cp -rpv \
    "${LIB_NAME}" \
    "${LIB_NAME}.h" \
    "${BUILD_DESTINATION}src/"

output Including documentation
log_command cp -rpv \
    "docs" \
    "${BUILD_DESTINATION}extras/"

output Creating default example
log_command mkdir -p "${BUILD_DESTINATION}examples/${LIB_NAME}"

output Creating "${BUILD_DESTINATION}examples/${LIB_NAME}/${LIB_NAME}.ino"
tee_command sed -rE 's/#include(\s+)\"'${LIB_NAME}'[.]h\"/#include\1<'${LIB_NAME}'.h>/' "${LIB_NAME}.ino" > "${BUILD_DESTINATION}examples/${LIB_NAME}/${LIB_NAME}.ino"

output Patching existing examples
for EXAMPLE in $(basename $(find examples/ -mindepth 1 -maxdepth 1 -type d -not -iname ".*")); do 
    FILE="examples/${EXAMPLE}/${EXAMPLE}.ino"
    output Patching "${FILE}" as "${BUILD_DESTINATION}${FILE}"
    tee_command sed -rE 's/#include(\s+)\"'${LIB_NAME}'[.]h\"/#include\1<'${LIB_NAME}'.h>/' "${FILE}" > "${BUILD_DESTINATION}${FILE}"
done;


output Changing to ${BUILD_BASE}
log_command cd "${BUILD_BASE}"

output Checking Release Mode
${RELEASE} || {
    log_command touch "${LIB_NAME}/.development"
    log_command touch "${EXPORT_FOLDER}/development_release"
    output "Development Mode (use RELEASE=true for release)"
}

output Compressing library
log_command zip -r "${EXPORT_FOLDER}/${LIB_NAME}.zip" "${LIB_NAME}" >> "${LOG_FILE}"
cd "${WORKING_FOLDER}"
echo Exited ${BUILD_BASE}

echo Removing Temporary Directory
rm -r "${BUILD_BASE}"

echo Snapshotting source

echo Finished building $LIB_NAME in $EXPORT_FOLDER