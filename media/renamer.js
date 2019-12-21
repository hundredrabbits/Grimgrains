const testFolder = '.';
const fs = require('fs');

fs.readdir('.', (err, files) => {
  files.forEach(file => {
    const ext = file.split('.')[file.split('.').length-1]
    const filename = file.replace(`.${ext}`,'').replace(/\./g,'_')

    fs.rename(file, filename+'.png', function(err) {
        if ( err ) console.log('ERROR: ' + err);
    });

    console.log(filename,ext);
  });
});