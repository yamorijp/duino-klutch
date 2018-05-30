const chakram = require("chakram")
const expect = chakram.expect
const jsend = require("../jsend")
const consts = require("../consts")
const host = consts.host


const stateSchema = {
  type: "object",
  required: ["status", "data"],
  properties: {
    status: {
      enum: ["success"]
    },
    data: {
      properties: {
        state: {
          required: true,
          type: "boolean"
        },
        updated: {
          required: true,
          type: "number"
        }
      }
    }
  }
}

describe("Switch API", () => {

  if (consts.handlers.relay) {

    after(() => {
      chakram.get(host + "/switch/relay/off")
      chakram.wait()
    })

    describe("GET /switch/relay/on", () => {
      it("should return success", () => {
        const r = chakram.get(host + "/switch/relay/on")
        return expect(r).to.have.schema(stateSchema)
      })
    })

    describe("GET /switch/relay/off", () => {
      it("should return success", () => {
        const r = chakram.get(host + "/switch/relay/off")
        return expect(r).to.have.schema(stateSchema)
      })
    })

    describe("GET /switch/relay/toggle", () => {
      it("should return success", () => {
        const r = chakram.get(host + "/switch/relay/toggle")
        return expect(r).to.have.schema(stateSchema)
      })
    })
  }

  if (consts.handlers.motion) {
    describe("GET /switch/motion/state", () => {
      it("should return state", () => {
        const r = chakram.get(host + "/switch/motion/state")
        return expect(r).to.have.schema(stateSchema)
      })
    })
  }
})
