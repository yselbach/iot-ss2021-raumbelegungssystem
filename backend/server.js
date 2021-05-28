const express = require('express')
const app = express()
const mongoose = require('mongoose')

const url = `mongodb+srv://PhilippNicoYannik:YannikNicoPhilipp@rooms.qyupo.mongodb.net/rooms?retryWrites=true&w=majority`;

const connectionParams={
    useNewUrlParser: true,
    useCreateIndex: true,
    useUnifiedTopology: true,
    useFindAndModify: false
}
mongoose.connect(url,connectionParams)
    .then( () => {
        console.log('Mit Datenbank verbunden')
    })
    .catch( (err) => {
        console.error(`Error connecting to the database. \n${err}`);
    })

app.use(express.json())

//const userRouter = require('./routes/person')
//app.use('/person', personRouter)

app.listen(3000, () => console.log('Server gestartet'))