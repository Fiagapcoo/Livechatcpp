const express = require('express');
const app = express();
const port = 8000;

const mysql = require('mysql2');

const connection = mysql.createConnection({
  host: 'localhost',
  user: 'root',
  password: '',
  database: 'livechat'
});


function underscore_to_space(string){
  return string.replace(/_/g, " ");
}


app.post('/', (req, res) => {
    var query = "SELECT * FROM chat ORDER BY idMsg";
    
    connection.query(query, (err, result) => {
        if(err) throw err;
        else{
            res.json(result);
            
        }
    });
});

app.post ('/enviar', (req, res) => {

  var cpp_vars = req.query.nome;

  //split the vars in |__| to get the name and the message
  var vars = cpp_vars.split("|");
  var nome = vars[0];
  var msg = vars[1];

  //
  nome = nome.substring(1);
  msg = msg.substring(0, msg.length-1);





  var msg2 = underscore_to_space(msg);

  var query = "INSERT INTO chat (idMsg, nome, content) VALUES (NULL, ?, ?)";

  connection.query(query, [nome, msg2], (err, result) => {
    if(err) throw err;
    else{
      res.json(result);
    }
  });
  
 


});


app.post('/apagar', (req, res) => {
  var query = "DELETE FROM chat";
  connection.query(query, (err, result) => {
    if(err) throw err;
    else{
      res.send(result);
    }
  });
});



app.listen(port, () => {
  console.log(`Example app listening at http://localhost:${port}`)
});

