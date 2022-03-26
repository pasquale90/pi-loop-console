#!/bin/bash

#define the path where the yml files are stored
envdir=$1
#provide some checks
if [[ -d "${envdir}" ]]
then
	echo "$envdir exists on your filesystem."
	if [[ "$envdir" != */ ]]
	then
		envdir=${envdir}/
	fi
else
	echo "No relative path ${newdir} found. Maybe try with an absolute path?"
fi


#list all yml files using their relative path
filepaths=$(ls -t ${envdir}*.yml)

#env will have the name of the file without the extension .yml
ext=".yml"
for filepath in $filepaths;do
	file=${filepath#$envdir}
	env=${file%$ext}
	echo "from file=${file} in filepath=${filepath}, a new environment is being created named ${env}"
	conda env update -n ${env} --file ${filepath}
done


