const express = require('express')
const router = express.Router()
const Room = require('../models/room')

//eigetnlich nicht nötig
router.get('/', async (req, res) => {
    try {
        const room = await Room.find()
        res.json(room)
    } catch(error) {
        console.log(error)
    }
})

//Infos über bestimmten Raum bekommen
router.get('/:roomId', getRoom, async(req, res) => {
    res.json(res.room)
})

//Raum zurücksetzen

//Personen betreten Raum

//Personen verlassen Raum
