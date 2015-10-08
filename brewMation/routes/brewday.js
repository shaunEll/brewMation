var express = require('express');
var moment = require('moment');

var router = express.Router();

/* GET brew days listing. */
router.get('/', function (req, res) {
  var db = req.db;
    var collection = db.get('brewDays');
    console.log(collection.find( ));
    collection.find({},{},function(e,docs){
        res.render('brewday', {
            title: 'BrewDays',
            "brewdays" : docs
        });
    });
});

/* GET view specific brewday. */
router.get('/view/:id', function(req , res){
  var db = req.db;
    var collection = db.get('brewDays');
    collection.findOne({'name':req.params.id},function(e,doc){
        console.log(doc);
        res.render('viewbrewday', {
            title: req.params.id,
            "diary" : doc
        });
    });
});

/* POST to Add diary entry */
router.post('/view/addEntry/:id', function(req, res) {
  console.log("Here");
    // Set our internal DB variable
    var db = req.db;

    // Get our form values. These rely on the "name" attributes
    var entryText = req.body.entryText;
    var entryTime = moment().toISOString();
    var entryImportant = req.body.entryImportant;

    // Set our collection
    var collection = db.get('brewDays');
    collection.update({'name':req.params.id},
        { "$addToSet": { entries: {
            "entry" : entryText,
            "timeStamp" : entryTime,
            "important" : entryImportant
          }}},function(err,data){
            if (err) {
                // If it failed, return error
                res.send("There was a problem adding the information to the database.");
            }
            else {
                // And forward to success page
                res.redirect('/brewday/view/' + req.params.id);
            }
        });
});

/* GET New brewday page. */
router.get('/new', function(req, res) {
    res.render('newbrew', { title: "Let's start a new brew!" });
});

module.exports = router;
