var express = require('express');
var moment = require('moment');

var router = express.Router();

/* GET brew days listing. */
router.get('/', function (req, res) {
  var db = req.db;
    var collection = db.get('recipes');
    console.log(collection.find( ));
    collection.find({},{},function(e,docs){
        res.render('recipe', {
            title: 'Recipes',
            "recipes" : docs
        });
    });
});

/* GET view specific brewday. */
router.get('/view/:id', function(req , res){
  var db = req.db;
    var collection = db.get('recipes');
    collection.findOne({'name':req.params.id},function(e,doc){
        console.log(doc);
        res.render('recipe_view', {
            title: req.params.id,
            "recipe" : doc
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
    var collection = db.get('recipes');
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
                res.redirect('/recipe/view/' + req.params.id);
            }
        });
});

/* GET New recipe page. */
router.get('/new', function(req, res) {
    res.render('recipe_new', { title: "Time to create a new recipe, yummy!" });
});

module.exports = router;
