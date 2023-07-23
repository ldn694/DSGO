@echo off
cd release
make || (
   echo build failed
   exit /b
)
echo build succeeded
DSGO || (
   echo run failed
   exit /b
)
echo run succeeded