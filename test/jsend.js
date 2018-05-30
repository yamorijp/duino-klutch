// JSend Spec
// https://labs.omniti.com/labs/jsend

exports.success = {
  type: "object",
  required: ["status", "data"],
  properties: {
    status: {
      enum: ["success"]
    }
  }
}

exports.fail = {
  type: "object",
  required: ["status", "data"],
  properties: {
    status: {
      enum: ["fail"]
    }
  }
}

exports.error = {
  type: "object",
  required: ["status", "message"],
  properties: {
    status: {
      enum: ["error"]
    },
    message: {
      type: "string"
    }
  }
}
