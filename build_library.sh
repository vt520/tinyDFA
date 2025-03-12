#!/bin/bash
PROJECT_FOLDER="${PWD}"
PROJECT_NAME=$(basename "${PROJECT_FOLDER}")
RELEASE=${RELEASE:-false}

TEMPORARY_FOLDER="$(mktemp -d --suffix=${PROJECT_NAME})"
LOG_FILE="${PROJECT_FOLDER}/.build.log"
CONFIGURATION="${PROJECT_FOLDER}/library.properties"

get_config() {
    VALUE=$(grep -E "^${1}=" "${CONFIGURATION}" || echo "$1=$2")
    echo $VALUE | tail -n 1 | sed -r "s/^[^=]+=//" 
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
parse_version() {
    echo $1 | sed -r "s/\./ /g"
}
major_version() {
    echo $1
}
minor_version() {
    echo $2
}
build_version() {
    echo $3
}

COPY_DESTINATION="${TEMPORARY_FOLDER}/${PROJECT_NAME}/"

[ -e "library.properties" ] || {
    output '"library.properties" is missing, are you in a Arduino project folder?'
    exit    
}


output "Getting Build Information"

VERSION=$(get_config version 0.0.0) 
echo $(get_config builder nobody)
PARSED_VERSION=$(parse_version "$VERSION")
MAJOR_VERSION=$(major_version $PARSED_VERSION)
MINOR_VERSION=$(minor_version $PARSED_VERSION)
BUILD_VERSION=$(( $(build_version $PARSED_VERSION) + 1 ))

VERSION="$MAJOR_VERSION.$MINOR_VERSION.$BUILD_VERSION"

output Updating library.properties
set_config version "$VERSION"
set_config sequence $(date -u "+%s")

RELEASE_FOLDER="${PROJECT_FOLDER}/release/${VERSION}/"

output Creating Release folder
mkdir -p "${RELEASE_FOLDER}"

output Snapshotting Release Source
log_command tar cvf \
    "${RELEASE_FOLDER}/${PROJECT_NAME}_${VERSION}.tar" \
    $(find . -type f \
        -not -iname "*.tar" \
        -not -iname "*.zip" \
        -not -wholename "./build/*" \
        -not -wholename "./.*" -not \
        -wholename "./release/*")

output Building Library ${PROJECT_NAME}, Version $VERSION as ${RELEASE_FOLDER}${PROJECT_NAME}.zip
log_command mkdir -p "${COPY_DESTINATION}src/${PROJECT_NAME}" 

output Including root files
log_command cp -rpv \
    "examples" \
    "keywords.txt" \
    "library.properties" \
    "LICENSE" \
    "README.md" \
    "${COPY_DESTINATION}"

output Including library source 
log_command cp -rpv \
    "${PROJECT_NAME}" \
    "${PROJECT_NAME}.h" \
    "${COPY_DESTINATION}src/"

output Including documentation
log_command cp -rpv \
    "docs" \
    "${COPY_DESTINATION}extras/"

output Creating default example
log_command mkdir -p "${COPY_DESTINATION}examples/${PROJECT_NAME}"

output Creating "${COPY_DESTINATION}examples/${PROJECT_NAME}/${PROJECT_NAME}.ino"
tee_command sed -rE 's/#include(\s+)\"'${PROJECT_NAME}'[.]h\"/#include\1<'${PROJECT_NAME}'.h>/' "${PROJECT_NAME}.ino" > "${COPY_DESTINATION}examples/${PROJECT_NAME}/${PROJECT_NAME}.ino"

output Patching existing examples
for EXAMPLE in $(basename $(find examples/ -mindepth 1 -maxdepth 1 -type d -not -iname ".*")); do 
    FILE="examples/${EXAMPLE}/${EXAMPLE}.ino"
    output Patching "${FILE}" as "${COPY_DESTINATION}${FILE}"
    tee_command sed -rE 's/#include(\s+)\"'${PROJECT_NAME}'[.]h\"/#include\1<'${PROJECT_NAME}'.h>/' "${FILE}" > "${COPY_DESTINATION}${FILE}"
done;


output Changing to ${TEMPORARY_FOLDER}
log_command cd "${TEMPORARY_FOLDER}"

output Checking Release Mode
${RELEASE} || {
    log_command touch "${PROJECT_NAME}/.development"
    log_command touch "${RELEASE_FOLDER}/development_release"
    output "Development Mode (use RELEASE=true for release)"
}

output Compressing library
log_command zip -r "${RELEASE_FOLDER}/${PROJECT_NAME}.zip" "${PROJECT_NAME}" >> "${LOG_FILE}"
cd "${PROJECT_FOLDER}"
echo Exited ${TEMPORARY_FOLDER}

echo Removing Temporary Directory
rm -r "${TEMPORARY_FOLDER}"

echo Snapshotting source

echo Finished building $PROJECT_NAME in $RELEASE_FOLDER