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

//Raum anlegen
router.post('/', async (req, res) => {
    const room = new Room ({
    roomNumber: req.body.number
    })
     try {      
        const newRoom = await room.save()
        res.status(201).json(newRoom)
    } catch (err) {
        res.status(400).json({ message: err.message })
    }
})

//Infos über bestimmten Raum bekommen
router.get('/:roomId', getRoom, async(req, res) => {
    res.json(res.room)
})

//Raum zurücksetzen


//Personen betreten Raum
router.put('/:id', getRoom, async (req, res) => {
    
let roomNumber = 

    try {
        if (req.room.roomNumber == null) {
            res.song.title = req.body.title
        }
        if (req.body.artist != null) {
            res.song.artist = req.body.artist
        }
        const updatedSong = await res.song.save()
        res.json(updatedSong)
    } catch(err) {
        res.status(400).json({ message: err.message})
    }
})


//Personen verlassen Raum
router.put('/:id', getRoom, async (req, res) => {
    try {
        if (req.body.title != null) {
            res.song.title = req.body.title
        }
        if (req.body.artist != null) {
            res.song.artist = req.body.artist
        }
        const updatedSong = await res.song.save()
        res.json(updatedSong)
    } catch(err) {
        res.status(400).json({ message: err.message})
    }
})


//Raum löschen
router.delete('/:id', getRoom, async (req, res) => {
    try {
        await res.room.remove()
        res.json({ roomNumber: res.room.roomNumber, message: 'Raum wurde gelöscht' })
    } catch(err) {
        res.status(500).json({ message: err.message})
    }
})




async function getRoom(req, res, next) {
    let room
    try {
        room = await Room.findById(req.params.id)
        if (room == null) {
            return res.status(404).json({ message: 'Kein Raum gefunden!'})
        }
    } catch(err) {
        return res.status(500).json({ message: err.message})
    }
    
    res.room = room
    next()
}

module.exports = router;