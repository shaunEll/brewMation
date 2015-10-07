var express = require('express');
var router = express.Router();

/* GET brew days listing. */
router.get('/', function (req, res) {
  var db = req.db;
    var collection = db.get('brewDays');
    console.log(collection.find( ));
    collection.find({},{},function(e,docs){
        res.render('brewday', { title: 'BrewDays',
            "brewdays" : docs
        });
    });
});

module.exports = router;
