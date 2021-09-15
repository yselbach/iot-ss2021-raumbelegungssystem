const mongoose = require('mongoose')

const roomSchema = new mongoose.Schema({
    /*roomId: {
        type: String,
        required: true,
        unique: true
    },*/
    roomNumber:{
        type: String,
        required: true,
        default: false
      },
    situation:{
        type: Boolean,
        required: true,
        default: false
    },
    personenAnzahl:{
        type: String,
        default: "0"
    }
    
})

module.exports = mongoose.model('Room', roomSchema)